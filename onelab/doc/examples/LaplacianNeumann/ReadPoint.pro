Include "LaplacianNeumann.pro";

//Post Operation
//==============
DefineConstant[ pointx = {0.4, Name "ReadPoint/pointx"} ];
DefineConstant[ pointy = {0.6, Name "ReadPoint/pointy"} ];
DefineConstant[ dataname = {"Output/Value", Name "ReadPoint/dataname"} ];

PostOperation{
  {Name readPoint; NameOfPostProcessing LaplacianNeumann;
    Operation{
			Print[u, OnPoint {pointx,pointy,0}, SendToServer dataname];
    }
  }
}
