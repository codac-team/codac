from pyibex_thickset import *
import gdal

from pyibex import IntervalVector, Interval, LargestFirst, cos, sin
import sys, os

def drawGeotiff(filename, figurename=''):
  try:
    gtiff = gdal.Open(filename)
  except:
    print("ERROR unable to open %s"%filename)
    exit(-1)
  # self.gtiff = gdal.Open( filename )
  geotransform = gtiff.GetGeoTransform()
  if figurename == '':
    figname = os.path.splitext(os.path.basename(filename))[0]
    print(figname)
    vibes.newFigure(figname)
    vibes.setFigureSize(500,500)
    vibes.setFigurePos(0,500)
  vibes.drawRaster(filename.replace("tiff", "png"), geotransform[0]+0.0*geotransform[1], geotransform[3]+0.0*geotransform[5], geotransform[1], geotransform[5])
  vibes.axisEqual()


X0 = IntervalVector(2, [-5.574, 5.203])

if True:
  t = ThickDisk(0,0,5)
  thickTest2Img(t, X0, 0.01, -0.01, "/tmp/disk.png")
# else:
vibes.beginDrawing()
vibes.newFigure("MAPS")
drawGeotiff("/tmp/disk.tiff", "MAPS")

vibes.drawCircle(0, 0, 5)
