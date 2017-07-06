#version 450
uniform sampler2D texture;
in vec2 v_texcoord;

uniform float sigmaS;
uniform float sigmaL;

#define EPS 1e-5

out vec4 FragColor;

float lum(in vec4 color) {
    return length(color.xyz);
}

void main()
{
  float sigS = max(sigmaS, EPS);
  float sigL = max(sigmaL, EPS);

  float facS = -1./(2.*sigS*sigS);
  float facL = -1./(2.*sigL*sigL);

  float sumW = 0.;
  vec4  sumC = vec4(0.);
  float halfSize = sigS * 2;
  ivec2 textureSize2 = textureSize(texture, 0);

  float l = lum(texture2D(texture,v_texcoord));

  for (float i = -halfSize; i <= halfSize; i ++){
    for (float j = -halfSize; j <= halfSize; j ++){
      vec2 pos = vec2(i, j);
      vec4 offsetColor = texture2D(texture, v_texcoord + pos / textureSize2);

      float distS = length(pos);
      float distL = lum(offsetColor)-l;

      float wS = exp(facS*float(distS*distS));
      float wL = exp(facL*float(distL*distL));
      float w = wS*wL;

      sumW += w;
      sumC += offsetColor * w;
    }
  }

  FragColor = sumC/sumW;
}
