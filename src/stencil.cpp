#include "stencil.hh"

Stencil::Stencil(Shader* stencilShader)
{
  _stencilShader = stencilShader;
}

void Stencil::drawOutline(Model* model, Shader* shaderScene, vec3f color, float thickness)
{
  (void) model;
  (void) shaderScene;
  (void) color;
  (void) thickness;

#if 0
  // 1st. render pass, draw objects as normal, writing to the stencil buffer
  // --------------------------------------------------------------------
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
  glStencilMask(0xFF);

  vec3f modelSz = model->size();

  shaderScene->use();
  model->draw(shaderScene);


  // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
  // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, 
  // thus only drawing the objects' size differences, making it look like borders.
  // --------------------------------------------------------------------    
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilMask(0x00);
  glDisable(GL_DEPTH_TEST);
  _stencilShader->use();
  _stencilShader->setVec3f("outlineColor", color);
  model->setSize(modelSz + (thickness / 10));
  model->draw(_stencilShader);

  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 0, 0xFF);
  glEnable(GL_DEPTH_TEST);

  model->setSize(modelSz);
#endif
}

