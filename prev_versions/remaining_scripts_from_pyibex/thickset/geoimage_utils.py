from pyibex import IntervalVector, Interval
from pyibex.thickset import * #PavingVisitor, IN, OUT, MAYBE_IN, MAYBE_OUT, UNK, MAYBE, EMPTY, ThickPaving, ThickDisk, ToVibes
from pyibex.thickset.thickimage import GeoImage, GeoMapper, ThickGeoImage
import numpy as np
import math

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



#
# if __name__ == '__main__':
#
# 	from vibes import vibes
# 	res = 0.002
# 	t = ThickDisk(Interval(0), Interval(0), Interval(0, 2), Interval(0,2))
# 	X0 = IntervalVector(2, [-10,10])
# 	P = ThickPaving(IntervalVector(2, [-10,10]), t, 0.01, opInter)
# 	# exportAsPNG(P, 0.01, -0.01, "test2.png")
# 	vibes.beginDrawing()
#
# 	thickTest2Img(t, X0, res, -res, "test2.png")
#
#
# 	P.visit(ToVibes(1000, 'test'))
# 	# exportAsPNG(P,0.005, -0.005)
# 	#
# 	img = misc.imread("test2.png", 0)
# 	img_in, img_out = grayToThickImg(img)
# 	timg = ThickGeoImage(img_in, img_out, -10,10, res, -res)
# 	#
# 	test = lambda x: opRestrict(timg.test(x), timg.test(x))
# 	test = lambda x: opRestrict(t.test(x), timg.test(x))
# 	P = ThickPaving(IntervalVector(2, [-10,10]), test, 0.01, opInter)
# 	# P.Sivia(t, 0.005, opRestrict)
# 	P.visit(ToVibes(1000, 'test2'))
#
# 	vibes.endDrawing()
