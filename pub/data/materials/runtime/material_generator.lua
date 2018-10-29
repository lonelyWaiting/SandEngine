g_shader_map = {}

function create_shader(createInfo)
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

function extract_texture_2d(str)
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

function parse_shader_body(str,shader_metaInfo)
    local texture_map, str = extract_texture_2d(str)

    shader_metaInfo["texture_sampler"] = texture_map
    shader_metaInfo["shader_content"]  = str
end

function dump_table(t,level)
    for k,v in pairs(t) do
        if type(v) == "table" then
            print(level .. k .. ": ")
            dump_table(v,level .. "     ")
        else
            if type(v) == "boolean" then
                if v == true then
                    print(level .. k .. " : " .. "true")
                else
                    print(level .. k .. " : " .. "false")
                end
            else
                print(level .. k .. ": " .. v)
            end
        end
    end
end

function create_state(create_info,state_name,shader_info)
    shader_info[state_name] = create_info
end

function parse_pass(str)
    shader_info = {}
    shader_info["vs_entry"] = string.match(str, "#pragma%s*vert%s*(%w+)")
    shader_info["ps_entry"] = string.match(str, "#pragma%s*fragment%s*(%w+)")

    local blend_state           = string.match(str, "(local%s*blend_state%s*=%s*\r\n%s*{%s*\r\n.-}%s*\r\n)")
    local rasterizer_state      = string.match(str, "(local%s*rasterizer_state%s*=%s*\r\n%s*{%s*\r\n.-}%s*\r\n)")
    local depth_stencil_state   = string.match(str, "(local%s*depth_stencil_state%s*=%s*\r\n%s*{%s*\r\n.-}%s*\r\n)")

    if blend_state ~= nil then
        MATDoString(blend_state .. "\r\ncreate_state(blend_state,'blend_state',shader_info)")
    end

    if rasterizer_state ~= nil then
        MATDoString(rasterizer_state .. "\r\ncreate_state(rasterizer_state,'rasterizer_state',shader_info)")
    end

    if depth_stencil_state ~= nil then
        MATDoString(depth_stencil_state .. "\r\ncreate_state(depth_stencil_state,'depth_stencil_state',shader_info)")
    end

    return shader_info
end

function parse_pipeline_state(str,shader_metaInfo)
    local pass_list = {}
    local i = 1
    str:gsub('Pass\r\n%b{}', function(w) pass_list[i] = w; i = i + 1 end )

    shader_metaInfo["pass_list"] = {}
    for i,v in ipairs(pass_list) do
        local pass_info = parse_pass(v)
        shader_metaInfo["pass_list"][i] = pass_info
    end
end

function generate_shader(str,filename)
    print("Begin Parse")
    local shader_metaInfo = {}
    for k,v in string.gmatch(str, "(.-)(Pass\r\n{.*})") do
        parse_shader_body(k,shader_metaInfo)
        parse_pipeline_state(v,shader_metaInfo)
    end
    print('End Parse')

    g_shader_map[filename] = shader_metaInfo

    dump_table(g_shader_map,"")
end