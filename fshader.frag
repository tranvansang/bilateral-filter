uniform sampler2D texture;
varying vec2 v_texcoord;

uniform float sigmaS;
uniform float sigmaL;

#define F 0.619928135
#define EPS 1e-10

vec4 filtering;

float lum(in vec4 color) {
    return length(color.xyz);
}

void main()
{
  float sigS = max(sigmaS, EPS);

  float facS = -1./(2.*sigS*sigS);
  float facL = -1./(2.*sigmaL*sigmaL);

  float sumW = 0.;
  vec4  sumC = vec4(0.);
  float halfSize = sigS / F;
  ivec2 textureSize2 = textureSize(texture, 0);

  float l = lum(texture(texture,v_texcoord));

  for (float i = -halfSize; i <= halfSize; i++){
    for (float j = -halfSize; j <= halfSize; j++){
      vec2 pos = vec2(i, j);
      vec4 offsetColor = texture2D(texture, v_texcoord + pos / textureSize2);

      float distS = length(pos);
      float distL = lum(col)-l;

      float wS = exp(facS*float(distS*distS));
      float wL = exp(facL*float(distL*distL));
      float w = wS*wL;

      sumW += w;
      sumC += offsetColor * w;
    }
  }

  filtering = sumC/sumW;
  gl_FragColor = filtering;
  //gl_FragColor = texture2D(texture, v_texcoord);
}

/*
#version 130
*/
