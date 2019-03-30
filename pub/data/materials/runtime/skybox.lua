function OnInit()
{
    local createInfo = 
    {
        vs_shader_file = "skybox.fx"
        ps_shader_file = "skybox.fx"
        gs_shader_file = "skybox.fx"
        ts_shader_file = "skybox.fx"
        cullMode = "point"
        depthTest = true

        -- blend state
        blendEnable           = true
        srcBlend              = MATBlendFactor.SrcAlpha
        destBlend             = MATBlendFactor.InvSrcAlpha
        blendOp               = MATBlendOperator.Add
        srcBlendAlpha         = MATBlendFactor.One
        destBlendAlpha        = MATBlendFactor.Zero
        blendOpAlpha          = MATBlendOperator.Add
        renderTargetWriteMask = 0xff

        -- rasterzier state
        fullMode = MATFullMode.Solid
        cullMode = MATFullMode.Back
        
    }
    
    CreateShader(createInfo)
}

function OnUpdate()
{

}