	
mTool use Tool.[one of the following]	
			Line2Line,
			Dithering,
			Vectorize,
            		Centerline,
			NoProcessing
        
mDirection and mFillDirection use Direction.[one of the following]
			None,
			Horizontal, Vertical, Diagonal,
			NewHorizontal, NewVertical, NewDiagonal,
			NewReverseDiagonal, NewGrid, NewDiagonalGrid,
			NewCross, NewDiagonalCross,
			NewSquares,
			NewZigZag,
			NewHilbert,
			NewInsetFilling,

mDitheringMode	use DitheringMode.[one of the following]
			Atkinson,
			FloydSteinberg,
			Burks,
			Jarvis,
			Random,
			Sierra2,
			Sierra3,
			SierraLight,
			Stucki

mFormula use Formula.[one of the following]
			SimpleAverage = 0,
			WeightAverage = 1,
			OpticalCorrect = 2,
			Custom = 3

mInterpolation use InterpolationMode.[one of the following]
			Invalid = -1,
        		Default = 0,
        		Low = 1,
        		High = 2,
        		Bilinear = 3,
        		Bicubic = 4,
        		NearestNeighbor = 5,
        		HighQualityBilinear = 6,
        		HighQualityBicubic = 7
		