function read_file(file_path)
    local f = assert(io.open(file_path, "rb"), "invalid file path" .. file_path)
    local content = f:read("*all")
    f:close()
    return content
end

function check_type(value)
    if type(value) == "number" then
        print("number")
    elseif type(value) == "string" then
        print("string")
    elseif type(value) == "function" then
        print("function")
    elseif type(value) == "boolean" then
        print("boolean")
    elseif type(value) == "nil" then
        print("nil")
    elseif type(value) == "table" then
        print("table")
    end
end

function read_properties(str)
    local prop_list = {}

    local properties = string.match(str, "Properties\r\n%s*(%b{})")
    if properties == nil then
        return prop_list
    end

    local prop = string.match(properties, "{(.*)}")
    for name, display_name, type, default_value in string.gmatch(prop, "%s*(.-)%(\"(.-)\", (.-)%)%s*=%s*\"(.-)\"") do
        local t = {}
        t["display_name"] = display_name
        t["type"] = type
        t["default_value"] = default_value
        prop_list[name] = t
    end
    return prop_list
end

-- split pass's content into parts and append different parts into one hlsl content
function read_single_pass(str, filename, pass_id)
    local pass_info = {}
    local pass = string.match(str, "HLSLPROGRAM(.-)ENDHLSL")
    if pass == nil then return pass_info end
    local tmp_str = pass

    pass_info["Name"] = string.match(pass, "Name%s+\"(.-)\"\r\n")
    tmp_str = tmp_str:gsub("Name%s+\"(.-)\"\r\n", "")

    pass_info["vertex_entry"]  = string.match(pass, "#pragma%s+vertex%s+(.-)\r\n")
    tmp_str = tmp_str:gsub("#pragma%s+vertex%s+.-\r\n", "")
    -- gsub的pattern参数传入的应为正则,即便传入字符串也会被当做正则

    pass_info["fragment_entry"] = string.match(pass, "#pragma%s+fragment%s+(.-)\r\n")
    tmp_str = tmp_str:gsub("(#pragma%s+fragment%s+.-\r\n)", "")

    pass_info["hull_entry"] = string.match(pass, "#pragma%s+hull%s+(.-)\r\n")
    tmp_str = tmp_str:gsub("(#pragma%s+hull%s+.-\r\n)", "")

    pass_info["domain_entry"] = string.match(pass, "#pragma%s+domain%s+(.-)\r\n")
    tmp_str = tmp_str:gsub("(#pragma%s+domain%s+.-\r\n)", "")

    pass_info["geometry_entry"] = string.match(pass, "#pragma%s+geometry%s+(.-)\r\n")
    tmp_str = tmp_str:gsub("(#pragma%s+geometry%s+.-\r\n)", "")

    local tex2D = {}
    local tex2D_index = 1
    for tex_name in string.gmatch(pass, "Texture2D%s+(.-);\r\n") do
        tex2D[tex2D_index] = tex_name
        tex2D_index = tex2D_index + 1
    end
    -- pass_info["texture2D"] = tex2D

    for i = 1, tex2D_index - 1, 1 do
        tmp_str = tmp_str:gsub("(Texture2D%s+.-;\r\n)", "")
    end

    -- TODO: 3D, TextureArray, Cube Texture Support

    local sampler = {}
    local sampler_index = 1
    for sampler_name in string.gmatch(pass, "sampler%s+(.-);\r\n") do
        -- TODO. support sampler meta data parse
        sampler[sampler_index] = sampler_name
        sampler_index = sampler_index + 1;
    end
    -- pass_info["sampler"] = sampler

    for i = 1, sampler_index - 1, 1 do
        tmp_str = tmp_str:gsub("(sampler%s+.-;\r\n)", "")
    end

    local shader_text = ""

    -- #define 
    local define_count = 0
    for define_exp in string.gmatch(pass, "(#define%s+.-%s+.-\r\n)") do
        shader_text = shader_text .. define_exp
        define_count = define_count + 1
    end
    for i = 1, define_count do
        tmp_str = tmp_str:gsub("(#define%s+(.-)%s+(.-)\r\n)", "")
    end

    -- texture and sampler
    for _, name in pairs(tex2D) do
        shader_text = shader_text .. "\r\n" .. "Texture2D " .. name .. ";"
    end

    for _, name in pairs(sampler) do
        shader_text = shader_text .. "\r\n" .. "sampler " .. name .. ";"
    end
    
    -- custom struct
    local struct_count = 0
    for struct_str in string.gmatch(pass, "(struct.-%b{};)\r\n") do
        shader_text = shader_text .. "\r\n" .. struct_str
        struct_count = struct_count + 1
    end

    for i = 1, struct_count, 1 do
        tmp_str = tmp_str:gsub("(struct.-%b{};\r\n)", "")
    end

    -- custom function
    local function_count = 0
    for function_str in string.gmatch(pass, "([^%s]+%s+[^%s]+%s*%b()%s*%b{})") do
        shader_text = shader_text .. "\r\n" .. function_str
        function_count = function_count + 1
    end

    for i = 1, function_count, 1 do
        tmp_str = tmp_str:gsub("([^%s]+%s+[^%s]+%s*%b()%s*%b{})", "")
    end

    function_count = 0
    for function_str in string.gmatch(pass, "([^%s]+%s+[^%s]+%s*%b()%s*:%s*.+%b{})") do
        shader_text = shader_text .. "\r\n" .. function_str
        tmp_str = tmp_str:gsub(function_str, "")
        function_count = function_count + 1
    end

    for i = 1, function_count, 1 do
        tmp_str = tmp_str:gsub("([^%s]+%s+[^%s]+%s*%b()%s*:%s*.+%b{})", "")
    end

    --  constant buffer
    if tmp_str ~= "" then
        shader_text = "cbuffer cb_" .. filename .. "_" .. pass_id .. "\r\n{" .. tmp_str .. "}\r\n" .. shader_text
    end

    pass_info["shader_text"] = shader_text
    return pass_info
end

function read_pass(str, filename)
    local pass_list = {}
    local i = 1
    for v in string.gmatch(str, "Pass%s*\r\n%s*(%b{})") do
        pass_list[i] = read_single_pass(v, filename, i)
        i = i + 1
    end

    return pass_list
end

function get_filename(path)
    path = string.gsub(path, "/", "\\")
    return string.match(path, ".-([^\\]-)%.[^\\%.]+")
end

function parse_shader(filename)
    local file_content = read_file(filename)

    shader_info = {}
    local shader_name, shader_content = string.match(file_content, "Shader%s+\"(.-)\"\r\n{(.*)}")
    local shader = {}
    shader["Properties"] = read_properties(shader_content)
    shader["Pass"]       = read_pass(shader_content, get_filename(filename))

    shader_info[shader_name] = shader

    for k, v in pairs(shader["Pass"]) do
        local pass_name = v["Name"]
        if pass_name == nil then pass_name = "" end
        
        local debug_name = shader_name .. pass_name

        local vertex_entry = v["vertex_entry"]
        if vertex_entry == nil then vertex_entry = "" end

        local pixel_entry = v["fragment_entry"]
        if pixel_entry == nil then pixel_entry = "" end

        local hull_entry = v["hull_entry"]
        if hull_entry == nil then hull_entry = "" end

        local domain_entry = v["domain_entry"]
        if domain_entry == nil then domain_entry = "" end

        local geometry_entry = v["geometry_entry"]
        if geometry_entry == nil then geometry_entry = "" end

        -- todo
        local keyword_list = {}

        compile(shader_name, 
                pass_name, 
                debug_name, 
                v["shader_text"], 
                vertex_entry, 
                pixel_entry, 
                hull_entry, 
                domain_entry, 
                geometry_entry,
                keyword_list)
    end

	shader_finish_compile(shader_name)

    return shader_name
end

-- parse_shader("../shaders/skybox.shader")

function parse_sampler(str)
    split_list = {};
    for match in (str.."_"):gmatch("(.-)".."_") do
        table.insert(split_list, match);
    end

    sampler_info = {}
    local len = #split_list
    if len == 2 and split_list[1] == "sampler" then
        sampler_info["follow_texture"] = true
        sampler_info["texture_name"] = split_list[2]
    else
        sampler_info["follow_texture"] = true
        sampler_info["filter"]  = 0
        sampler_info["address"] = 0
        sampler_info["comparison"] = 0

        for _, item in pairs(split_list) do
            local lower_item = string.lower(item)

            if filter_mapping[lower_item] ~= nil then
                sampler_info["filter"] = filter_mapping[lower_item]
            elseif address_mapping[lower_item] ~= nil then
                sampler_info["address"] = address_mapping[lower_item]
            elseif comparison_mapping[lower_item] ~= nil then
                sampler_info["comparison"] = comparison_mapping[lower_item]
            end
        end
    end

    return sampler_info
end