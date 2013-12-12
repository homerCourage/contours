from Freestyle import *
from logical_operators import *
from PredicatesB1D import *
from shaders import *

seed = 1
N = 2


Operators.select(QuantitativeInvisibilityUP1D(0))
Operators.bidirectionalChain(ChainSilhouetteIterator(), NotUP1D(QuantitativeInvisibilityUP1D(0)))
shaders_list = 	[
#		BezierCurveShader(3),
#		pySamplingShader(2),
#		ConstantColorShader(1,1,1),
		pyIncreasingRandomColorShader(seed,N),
                pyConstantThicknessShader(4),
#pyNonLinearVaryingThicknessShader(2,8,0.6), 
#		pyMaterialColorShader(0)
		]
Operators.create(TrueUP1D(), shaders_list)
