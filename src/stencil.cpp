#include "stencil.hh"

Stencil::Stencil(Shader* shaderOutline) : _shaderOutline{shaderOutline}
{
  color = vec3f(1.0f, 0.0f, 0.0f); // default red
  thickness = 0.05f;
}

void Stencil::drawOutline(Model* model, Shader* shaderScene)
{
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
  _shaderOutline->use();
  _shaderOutline->setVec3f("color", color);
  model->setSize(modelSz+thickness);
  model->draw(_shaderOutline);

  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 0, 0xFF);
  glEnable(GL_DEPTH_TEST);

  model->setSize(modelSz);
}

