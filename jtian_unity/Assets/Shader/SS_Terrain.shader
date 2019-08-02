Shader "CFGE/SS_Terrain"
{
    Properties
    {
        _MainTex ("Albedo (RGB)", 2D) = "white" {}
        _DetailTex1 ("Albedo (RGB)", 2D) = "white" {}
        _DetailTex2 ("Albedo (RGB)", 2D) = "white" {}
        _SplatTex ("Splat Texture (RGB)", 2D) = "white" {}
        _Glossiness ("Smoothness", Range(0,1)) = 0.5
    }
    SubShader
    {
        Tags { 
			"RenderType"="Opaque"
			 }

        LOD 200

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface surf Standard fullforwardshadows

        // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        struct Input
        {
            float2 uv_MainTex : TEXCOORD0;
            float2 uv_DetailTex1 : TEXCOORD1;
            float2 uv_DetailTex2 : TEXCOORD2;
            float2 uv_SplatTex : TEXCOORD3;
        };

		sampler2D _MainTex;
		sampler2D _DetailTex1;
		sampler2D _DetailTex2;
		sampler2D _SplatTex;
		
		half _Glossiness;
        fixed4 _Color;

        // Add instancing support for this shader. You need to check 'Enable Instancing' on materials that use the shader.
        // See https://docs.unity3d.com/Manual/GPUInstancing.html for more information about instancing.
        // #pragma instancing_options assumeuniformscaling
        UNITY_INSTANCING_BUFFER_START(Props)
            // put more per-instance properties here
        UNITY_INSTANCING_BUFFER_END(Props)

        void surf (Input IN, inout SurfaceOutputStandard o)
        {
			fixed4 splat_control = tex2D(_SplatTex, IN.uv_SplatTex);
			fixed3 col = fixed3(0, 0, 0);

			float4 tmp = tex2D(_MainTex, IN.uv_MainTex);
			col = lerp(col, tmp.rgb, splat_control.r * tmp.a);
			tmp = tex2D(_DetailTex1, IN.uv_DetailTex1);
			col = lerp(col, tmp.rgb, splat_control.g * tmp.a);
			tmp = tex2D(_DetailTex2, IN.uv_DetailTex2);
			col = lerp(col, tmp.rgb, splat_control.b * tmp.a);

			o.Albedo = col;
            o.Smoothness = _Glossiness;
        }
        ENDCG
    }
    FallBack "Diffuse"
}
