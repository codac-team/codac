from pyibex import IntervalVector, Interval, integer
from pyibex.thickset import * #PavingVisitor, IN, OUT, MAYBE_IN, MAYBE_OUT, UNK, MAYBE, EMPTY, ThickPaving, ThickDisk, ToVibes
from pyibex.thickset.image import ThickGeoImage
from vibes import vibes
import numpy as np
import gdal
from scipy import ndimage
from scipy import misc
import math
from collections import deque
import os

def grayToThickImg(gray_img, in_val=255, maybe_val=128, out_val=0 ):
	""" Compute Integral image to X- and X+ sets

	Args:
	 	gray_img( np.array ): gray image with only three value
													255 : pixels which belong to X-
													128 : pixels which belong to X+
													0 : pixels outside X+
	"""

	img_in = np.zeros(gray_img.shape, dtype=np.uint64)
	img_in[gray_img == in_val] = 1
	img_in = img_in.cumsum(0).cumsum(1)

	img_out = np.zeros(gray_img.shape, dtype=np.uint64)
	img_out[gray_img >= maybe_val] = 1
	img_out = img_out.cumsum(0).cumsum(1)

	return (img_in, img_out)


def tiff2ThickGeoImage(filename, transpose=True, fliplr=False, **kwargs):
  """NOTE: Pixel is area or not ???
  """
  try:
    gtiff = gdal.Open(filename)
  except:
    print("ERROR unable to open %s"%filename)

  img = (gtiff.GetRasterBand(1).ReadAsArray())#.astype(np.uint64)
  if fliplr == True:
    print('fliplr')
    img=np.fliplr(img)
  if transpose == True:
    img=img.T

  img_in, img_out = grayToThickImg(img, **kwargs)


  geoTranform = list(gtiff.GetGeoTransform())

  print(geoTranform)
  metaData = gtiff.GetMetadata()
  if 'X' in metaData and 'Y' in metaData:
    x0 = float(metaData['X'])
    y0 = float(metaData['Y'])
    print("X: ", x0, "Y: ", y0 )
    geoTranform[0] -= y0
    geoTranform[3] -= x0

   # t = float(metaData['TIMESTAMP'])
  # thicktest = ThickGeoImage(img_in, img_out, geoTranform[0]+geoTranform[1]/2, geoTranform[3]+geoTranform[5]/2, geoTranform[1], geoTranform[5])
  thicktest = ThickGeoImage(img_in, img_out, geoTranform[0], geoTranform[3], geoTranform[1], geoTranform[5], outerVal=MAYBE)

  thicktest.timestamp=float(gtiff.GetMetadata().get('TIMESTAMP', -1))
  # thicktest.X0=float(gtiff.GetMetadata().get('X', np.nan))
  # thicktest.Y0=float(gtiff.GetMetadata().get('Y', np.nan))

  print(thicktest.timestamp)

  return thicktest


def exportAsPNG(paving, xres, yres, fileout):
  boundingBox = paving.X0
  extractIN = Extractor([IN])
  extractUNK = Extractor([MAYBE, MAYBE_IN, MAYBE_OUT, UNK])
  paving.visit(extractIN)
  paving.visit(extractUNK)


  nx, ny = math.ceil(boundingBox[0].diam()/xres), math.ceil(boundingBox[1].diam()/abs(yres))
  print(nx, ny)
  array = np.zeros((nx, ny))
  mapper = GeoMapper(boundingBox[0].lb(), boundingBox[1].ub(),nx, ny, xres, yres )


  for b in extractUNK.L:
    cords = mapper.world_to_grid(b)
    array[cords[0]:cords[1], cords[2]:cords[3]] = 128

  for b in extractIN.L:
    cords = mapper.world_to_grid(b)
    array[cords[0]:cords[1], cords[2]:cords[3]] = 255

  misc.imsave(fileout, array)


def coord_size(c):
  return max(c[1] - c[0], c[3] - c[2])

def split(coords):
  dx = int(coords[1] - coords[0])
  dy = int(coords[3] - coords[2])
  if (dx > dy and dx != 1):
    return ([coords[0],coords[0]+int(dx/2), coords[2], coords[3] ] , [coords[0]+int(dx/2)+1,coords[1], coords[2], coords[3] ])
  else:
    return ([coords[0],coords[1], coords[2], coords[2]+int(dy/2) ] , [coords[0],coords[1], coords[2]+int(dy/2)+1, coords[3] ])

def thickTest2Img(test, boundingBox, xres, yres, fileout):

  boundingBox[0] = integer(boundingBox[0]/xres)*xres
  boundingBox[1] = integer(boundingBox[1]/yres)*yres
  # lbx, ubx = np.floor(boundingBox[0].lb() / xres) * xres, np.ceil(boundingBox[0].ub() / xres) * xres
  # lby, uby = np.floor(boundingBox[1].lb() / yres) * yres, np.ceil(boundingBox[1].ub() / yres) * yres

  # boundingBox = Inter:valVector([[lbx, ubx], [lby, uby]])
  print(boundingBox,boundingBox.diam())

  nx, ny = math.ceil(boundingBox[0].diam()/abs(xres)), math.ceil(boundingBox[1].diam()/abs(yres))
  print(nx, ny)
  array = np.zeros((nx, ny))
  mapper = GeoMapper(boundingBox[0].lb(), boundingBox[1].ub(),nx, ny, xres, yres )

  stack =  deque([[0, nx-1, 0, ny-1]])

  while len(stack) > 0:
    c =    stack.popleft()
    # print(c)
    box = mapper.grid_to_world(c)
    t = test.test(box)
    # print(box, t, c)
    if t == IN:
      array[c[0]:c[1]+1, c[2]:c[3]+1] = 255
    elif t == MAYBE:
      array[c[0]:c[1]+1, c[2]:c[3]+1] = 128
    elif t == OUT:
      continue
    elif coord_size(c) > 1 and not is_singleton(t):
      c1, c2 = split(c)
      stack.append(c1)
      stack.append(c2)
    else :
      array[c[0]:c[1]+1, c[2]:c[3]+1] = 128
      # pass

  misc.imsave(fileout, array.T)
  basename, ext = os.path.splitext(fileout)
  wld_file_name = fileout + "w"
  with open(wld_file_name, "w") as fid:
    fid.write("%f\n0\n0\n%f\n%f\n%f"% (xres,yres,boundingBox[0].lb()+0.5*xres, boundingBox[1].ub()+0.5*yres) )

  print("to generate tiff file run:")
  print("gdal_translate -of GTIFF %s %s -a_nodata 0 -b 1"%(fileout, fileout.replace("png","tiff")))





if __name__ == '__main__':
  res = 0.002
  t = ThickDisk(Interval(0), Interval(0), Interval(0, 2), Interval(0,2))
  X0 = IntervalVector(2, [-10,10])
  P = ThickPaving(IntervalVector(2, [-10,10]), t, 0.01, opInter)
  # exportAsPNG(P, 0.01, -0.01, "test2.png")
  vibes.beginDrawing()

  thickTest2Img(t, X0, res, -res, "test2.png")


  P.visit(ToVibes(1000, 'test'))
  # exportAsPNG(P,0.005, -0.005)
  #
  img = misc.imread("test2.png", 0)
  img_in, img_out = grayToThickImg(img)
  timg = ThickGeoImage(img_in, img_out, -10,10, res, -res)
  #
  test = lambda x: opRestrict(timg.test(x), timg.test(x))
  test = lambda x: opRestrict(t.test(x), timg.test(x))
  P = ThickPaving(IntervalVector(2, [-10,10]), test, 0.01, opInter)
  # P.Sivia(t, 0.005, opRestrict)
  P.visit(ToVibes(1000, 'test2'))

  vibes.endDrawing()
