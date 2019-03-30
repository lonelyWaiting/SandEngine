MATBlendOperator =
{
    Add         = 0,
    Subtract    = 1,
    RevSubtract = 2,
    Min         = 3,
    Max         = 4
}

MATBlendFactor = 
{
    Zero            = 0,
    One             = 1,
    SrcColor        = 2,
    InvSrcColor     = 3,
    SrcAlpha        = 4,
    InvSrcAlpha     = 5,
    DestAlpha       = 6,
    InvDestAlpha    = 7,
    DestColor       = 8,
    InvDestColor    = 9,
    SrcAlphaSAT     = 10,
    BlendFactor     = 11,
    InvBlendFactor  = 12,
    Src1Color       = 13,
    InvSrc1Color    = 14,
    Src1Alpha       = 15,
    InvSrc1Alpha    = 16
}

MATFullMode =
{
    Wireframe   = 0,
    Solid       = 1
}

MATCullMode =
{
    None    = 0,
    Front   = 1,
    Back    = 2
}

MATDepthWriteMask =
{
    Zero    = 0,
    All     = 1,
}

MATComparisonFunc =
{
    Never           = 0,
    Less            = 1,
    Equal           = 2,
    LessEqual       = 3,
    Greater         = 4,
    NotEqual        = 5,
    GreaterEqual    = 6,
    Always          = 7
}

MATStencilOperator =
{
    Keep    = 0,
    Zero    = 1,
    Replace = 2,
    IncrSAT = 3,
    DecrSAT = 4,
    Invert  = 5,
    Incr    = 6,
    Decr    = 7
}