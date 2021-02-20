precision mediump float;
varying vec2 v_texCoord;
varying vec4 v_texColor;
vec4 v_textureColor;
uniform sampler2D s_texture;
uniform int b_UseTexture;
void main()
{
   if( b_UseTexture != 0 ) {
       v_textureColor = texture2D(s_texture, v_texCoord);
       gl_FragColor = v_textureColor*v_texColor;
   }  else
       gl_FragColor = v_texColor;
}