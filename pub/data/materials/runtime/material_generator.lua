function CreateShader(createInfo)
    local shaderInfo = {}

    shaderInfo.blend_state = {}
    shaderInfo.blend_state.blendEnable      = creatorInfo.blendEnable or true
    shaderInfo.blend_state.srcBlend         = createInfo.srcBlend or MATBlendFactor.SrcAlpha
    shaderInfo.blend_state.destBlend        = createInfo.destBlend or MATBlendFactor.InvSrcAlpha
    shaderInfo.blend_state.blendOp          = createInfo.blendOp or MATBlendOperator.Add
    shaderInfo.blend_state.srcBlendAlpha    = createInfo.srcBlendAlpha or MATBlendFactor.One
    shaderInfo.blend_state.destBlendAlpha   = createInfo.dstBlendAlpha or MATBlendFactor.Zero
    shaderInfo.blend_state.blendOpAlpha     = createInfo.blendOpAlpha or MATBlendOperator.Add

    shaderInfo.rasterizer_state = {}
    shaderInfo.rasterizer_state.fillMode                = createInfo.fillMode or MATFillMode.Solid
    shaderInfo.rasterizer_state.cullMode                = createInfo.cullMode or MATCullMode.Back
    shaderInfo.rasterizer_state.frontCounterClockwise   = createInfo.frontCounterClockwise or false
    shaderInfo.rasterizer_state.depthBias               = createInfo.depthBias or 0
    shaderInfo.rasterizer_state.depthBiasClamp          = createInfo.depthBiasClamp or 0.0
    shaderInfo.rasterizer_state.slopeScaleDepthBias     = createInfo.slopeScaleDepthBias or 0.0
    shaderInfo.rasterizer_state.depthClipEnable         = createInfo.depthClipEnable or true
    shaderInfo.rasterizer_state.scissorEnable           = createInfo.scissorEnable or true
    shaderInfo.rasterizer_state.multiSampleEnable       = createInfo.multiSampleEnable or false
    shaderInfo.rasterizer_state.antialiasedLineEnable   = createInfo.antialiasedLineEnable or false

    shaderInfo.depth_stencil_state = {}
    shaderInfo.depth_stencil_state.depthEnable      = createInfo.depthEnable or true
    shaderInfo.depth_stencil_state.depthWriteMask   = createInfo.depthWriteMask or MATDepthWriteMask.All
    shaderInfo.depth_stencil_state.depthFunc        = createInfo.depthFunc or MATComparisonFunc.LessEqual
    shaderInfo.depth_stencil_state.stencilEnable    = createInfo.stencilEnable or false
    shaderInfo.depth_stencil_state.stencilReadMask  = createInfo.stencilReadMask or 0xff
    shaderInfo.depth_stencil_state.stencilWriteMask = createInfo.stencilWriteMask or 0xff
    shaderInfo.depth_stencil_state.frontFace        = {}
    shaderInfo.depth_stencil_state.frontFace.stencilFailOp      = createInfo.frontFace_stencilFailOp or MATStencilOperator.Keep
    shaderInfo.depth_stencil_state.frontFace.stencilDepthFailOp = createInfo.frontFace_stencilDepthFailOp or MATStencilOperator.Keep
    shaderInfo.depth_stencil_state.frontFace.stencilPassOp      = createInfo.frontFace_stencilPassOp or MATStencilOperator.Keep
    shaderInfo.depth_stencil_state.frontFace.stencilFunc        = createInfo.frontFace_stencilFunc or MATComparisonFunc.Always
    shaderInfo.depth_stencil_state.backFace         = {}
    shaderInfo.depth_stencil_state.backFace.stencilFailOp       = createInfo.backFace_stencilFailOp or MATStencilOperator.Keep
    shaderInfo.depth_stencil_state.backFace.stencilDepthFailOp  = createInfo.backFace_stencilDepthFailOp or MATStencilOperator.Keep
    shaderInfo.depth_stencil_state.backFace.stencilPassOp       = createInfo.backFace_stencilPassOp or MATStencilOperator.Keep
    shaderInfo.depth_stencil_state.backFace.stencilFunc         = createInfo.backFace_stencilFunc or MATComparisonFunc.Always
end

function ExtractTexture2D(str)
    local texture_map = {}

    for k,v in string.gmatch(str, "Texture2D (%w+)<(.-)>") do
        local desc = {}
        print("k:" .. k .. "v:" .. v)
        for _k,_v in string.gmatch(v, "(%w+)%s*=%s*(.-);") do
            desc[_k] = _v
        end
        texture_map[k] = desc
    end

    str = string.gsub(str,"(<.->)",";")

    return texture_map, str
end

function ParseShaderBody(str)
    local texture_map, str = ExtractTexture2D(str)
end

function ParsePipelineState(str)
    local pass_list = {}
    i = 1
    str:gsub('Pass\r\n%b{}', 
    function(w) 
        pass_list[i] = w 
        i = i + 1
    end )
    
    print("pass_list element count:" .. #pass_list)
    for i,v in ipairs(pass_list) do
        print("Pass " .. i .. ':\r\n' .. pass_list[i])
    end
end

function GenerateShader(str)
    print("Begin Parse")
    for k,v in string.gmatch(str, "(.-)(Pass\r\n{.*})") do
        ParseShaderBody(k)
        ParsePipelineState(v)
    end
    print('End Parse')
end