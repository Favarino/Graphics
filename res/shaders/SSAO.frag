#version 330;
//kernelsize

void main()
{
for (int i = 0; i < kernelSize; ++i) {
   kernel[i] = vec3(
   random(-1.0f, 1.0f),
   random(-1.0f, 1.0f),
   random(0.0f, 1.0f)
   kernel[i].normalize();
   kernel[i] *= random(0.0f,1.0f);
}

float scale = float(i) / float(kernelSize);

   scale = lerp(0.1f, 1.0f, scale * scale);
   kernel[i] *= scale;

   for (int i = 0; i < noiseSize; ++i) {
   noise[i] = vec3(
      random(-1.0f, 1.0f),
      random(-1.0f, 1.0f),
      0.0f
   );
   noise[i].normalize();
}
}
