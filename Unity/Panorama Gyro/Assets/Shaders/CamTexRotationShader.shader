/**
* WebcamTexture gets flipped 90 degrees on mobile. This shader will apply a rotation matrix
* in order to correct this redundant flip.
*/
Shader "Custom/CamTexRotationShader"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
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
                float4 vertex : POSITION; // float4
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;

            v2f vert (appdata v)
            {
                /**
                * Rotating the texture 90 degrees using a rotation matrix:
                * each vertex (x, y) will be transformed to (xcos(t)-ysin(t), xsin(t)+ycos(t))
                */

                float2x2 rotationMatrix = float2x2(
                    cos(-90), -sin(-90), 
                    sin(-90), cos(-90)
                );

                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                o.uv = mul(o.uv, rotationMatrix);
                return o;
            }

            // runs for every pixle
            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
                return col;
            }
            ENDCG
        }
    }
}
