#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform sampler2D texture1;

varying vec2 v_texcoord;

uniform bool isNight;

uniform int method;
// 亮度
uniform float brightness;
// 对比度
uniform float contrast;
// 饱和度
uniform float saturation;
// 色阶
uniform float highLight;
uniform float shadow;
uniform float midtone;
uniform float coefficient;

uniform float globalRgbMax;
uniform float globalRgbMin;

//! [0]
void main()
{
    // Set fragment color from texture

    // 图像编辑
    // 相机图像

    vec4 srcColor = texture2D(texture, v_texcoord);
    vec4 beamColor = texture2D(texture1, v_texcoord);
    if( isNight ){

        if( method == 1 ){
            float rgbMax = max(max(srcColor.r, srcColor.g), srcColor.b);
            float rgbMin = min(min(srcColor.r, srcColor.g), srcColor.b);

            // 调节亮度
            srcColor = vec4(clamp(srcColor.rgb + brightness, 0.0, 1.0),srcColor.a);

            // 调节对比度
            vec3 contrastColor = srcColor.rgb + (srcColor.rgb - vec3(0.5)) * contrast;
            srcColor = vec4(clamp( contrastColor , 0.0 , 1.0 ), srcColor.a);

            float delta = rgbMax - rgbMin;
            float value = rgbMax + rgbMin;
            // HSL
            float L = value / 2.0;
            float S;
            if (L < 0.5) {
                S = delta / value;
            } else {
                S = delta / (2.0 - value);
            }

            float alpha;
            vec3 saturationColor;

            if (saturation < 0.0) {
                alpha = saturation;
                saturationColor = vec3(L) + (srcColor.rgb - vec3(L)) * (1.0 + alpha);
            } else {
                if (saturation + S >= 1.0) {
                    alpha = S;
                } else {
                    alpha = 1.0 - saturation;
                }
                alpha = 1.0 / alpha - 1.0;
                saturationColor = srcColor.rgb + (srcColor.rgb - vec3(L)) * alpha;
            }
            srcColor = vec4(clamp(saturationColor, 0.0, 1.0), srcColor.a);
        }else if( method == 2 ){

            float diff = highLight - shadow;
            vec3 rgbDiff = max(srcColor.rgb - shadow, 0.0);
            srcColor = vec4(clamp(pow(rgbDiff / diff, vec3(1.0 / midtone)), 0.0, 1.0) , srcColor.a);

        }else{

            float a = 1.0 - exp(pow(pow(-1.0 + coefficient,2.0),2.0));
            float b = 1.0 - exp(pow(pow(-globalRgbMin / globalRgbMax + coefficient, 2.0), 2.0));
            float c = globalRgbMax * (1.0 - b) / (a - b);
            float d = globalRgbMax / (a - b);

            vec3 resultColor = -srcColor.rgb / globalRgbMax + coefficient ;

            resultColor = pow(resultColor,vec3(2.0));
            resultColor = pow(resultColor,vec3(2.0));
            resultColor = c - d*exp(resultColor);

            srcColor = vec4(clamp(resultColor, 0.0, 1.0), srcColor.a);
        }

        gl_FragColor = mix(srcColor, beamColor, 0.3);
    }else{
        gl_FragColor = mix(srcColor, beamColor, 0.3);
    }

}
//! [0]

