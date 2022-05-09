




RasterizerState	RS_Default
{
	FillMode = Solid;
	FrontCounterClockwise = false;
	CullMode = back;
};

RasterizerState	RS_WireFrame
{
	FillMode = WireFrame;
};

RasterizerState	RS_Cull_CW
{
	FillMode = Solid;
	FrontCounterClockwise = false;
	CullMode = front;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

BlendState BS_NonBlend
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;
	SrcBlend[0] = Src_Alpha;
	DestBlend[0] = Inv_Src_Alpha;
	BlendOp[0] = Add;
};
