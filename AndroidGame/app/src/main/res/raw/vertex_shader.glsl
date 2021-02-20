attribute vec4 vPosition;
attribute vec2 vtexCoord;
attribute vec4 vColor;
uniform mat4 u_ProjMatrix;
uniform mat4 u_ModelMatrix;
varying vec2 v_texCoord;
varying vec4 v_texColor;
void main()
{
   gl_Position = u_ProjMatrix*u_ModelMatrix*vPosition;
   v_texCoord = vtexCoord;
   v_texColor = vColor;
}