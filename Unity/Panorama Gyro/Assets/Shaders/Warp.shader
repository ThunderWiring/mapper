Shader "Custom/Warp"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {
        Tags { "RenderType" = "Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                UNITY_FOG_COORDS(1)
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;
            float4x4 _WarpMatrix;

            v2f vert(appdata v)
            {
                // take the Homography matrix from the 4x4 matrix, by ignoring the top row and leftmost col
                float3x3 H = float3x3(
                    _WarpMatrix[1][1], _WarpMatrix[1][2], _WarpMatrix[1][3],
                    _WarpMatrix[2][1], _WarpMatrix[2][2], _WarpMatrix[2][3],
                    _WarpMatrix[3][1], _WarpMatrix[3][2], _WarpMatrix[3][3]
                    );
                v2f o;
                float3 x = float3(v.uv, 1);
                float3 warped = mul(H, x);
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = float2(warped.x, warped.y);

                return o;
            }

            fixed4 frag(v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
                return col;
            }
            ENDCG
        }

    }
}
