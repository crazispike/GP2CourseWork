//
//  Material.cpp
//  GP2BaseCode
//
//  Created by james glasgow on 23/11/2015.
//
//
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

Material::Material()
{
  m_ShaderProgram=0;
  m_AmbientMaterial = vec4(0.3f,0.3f,0.3f,1.0f);
  m_DiffuseMaterial=vec4(0.8f,0.8f,0.8f,1.0f);
  m_SpecularMaterial=vec4(1.0f,1.0f,1.0f,1.0f);
  m_SpecularPower=25.0f;
  m_DiffuseMap = 0;
  m_SpecularMap=0;

  m_EnvironmentMap=0;
}

Material::~Material()
{
  glDeleteProgram(m_ShaderProgram);
  glDeleteTextures(1, &m_DiffuseMap);
  glDeleteTextures(1, &m_SpecularMap);
  glDeleteTextures(1,&m_EnvironmentMap);
}

void Material::loadShader(const string& vsFilename, const string& fsFilename)
{
  GLuint vertexShaderProgram = 0;
  vertexShaderProgram = loadShaderFromFile(vsFilename, VERTEX_SHADER);
  checkForCompilerErrors(vertexShaderProgram);
  
  GLuint fragmentShaderProgram = 0;
  fragmentShaderProgram = loadShaderFromFile(fsFilename, FRAGMENT_SHADER);
  checkForCompilerErrors(fragmentShaderProgram);
  
  m_ShaderProgram = glCreateProgram();
  glAttachShader(m_ShaderProgram, vertexShaderProgram);
  glAttachShader(m_ShaderProgram, fragmentShaderProgram);
  
  //Link attributes
  glBindAttribLocation(m_ShaderProgram, 0, "vertexPosition");
  glBindAttribLocation(m_ShaderProgram, 1, "vertexColour");
  glBindAttribLocation(m_ShaderProgram, 2, "vertexTexCoords");
  glBindAttribLocation(m_ShaderProgram, 3, "vertexNormal");
  glBindAttribLocation(m_ShaderProgram, 7, "instancePosition");
  
  glLinkProgram(m_ShaderProgram);
  checkForLinkErrors(m_ShaderProgram);
  //now we can delete the VS & FS Programs
  glDeleteShader(vertexShaderProgram);
  glDeleteShader(fragmentShaderProgram);
  setupUniforms();
}

void Material::setupUniforms()
{
  glUseProgram(m_ShaderProgram);
  GLint MVPLocation = glGetUniformLocation(m_ShaderProgram, "MVP");
  
  GLint ambientLightColourLocation = glGetUniformLocation(m_ShaderProgram, "ambientLightColour");
  GLint ambientMaterialColourLocation = glGetUniformLocation(m_ShaderProgram, "ambientMaterialColour");
  
  GLint diffuseLightColourLocation = glGetUniformLocation(m_ShaderProgram, "diffuseLightColour");
  GLint diffuseLightMaterialLocation = glGetUniformLocation(m_ShaderProgram, "diffuseMaterialColour");
  GLint lightDirectionLocation = glGetUniformLocation(m_ShaderProgram, "lightDirection");
  
  GLint specularLightColourLocation = glGetUniformLocation(m_ShaderProgram, "specularLightColour");
  GLint specularLightMaterialLocation = glGetUniformLocation(m_ShaderProgram, "specularMaterialColour");
  GLint specularPowerLocation = glGetUniformLocation(m_ShaderProgram, "specularPower");
  GLint cameraPositionLocation = glGetUniformLocation(m_ShaderProgram, "cameraPosition");
  
  GLint modelLocation = glGetUniformLocation(m_ShaderProgram, "Model");
  
  GLint texture0Location = glGetUniformLocation(m_ShaderProgram, "texture0");
 
  m_UniformLocationMap["MVP"] = MVPLocation;
  m_UniformLocationMap["ambientLightColour"] = ambientLightColourLocation;
  m_UniformLocationMap["ambientMaterialColour"] = ambientMaterialColourLocation;
  m_UniformLocationMap["diffuseLightColour"] = diffuseLightColourLocation;
  m_UniformLocationMap["diffuseMaterialColour"] = diffuseLightMaterialLocation;
  m_UniformLocationMap["lightDirection"] = lightDirectionLocation;
  m_UniformLocationMap["specularLightColourLocation"] = specularLightColourLocation;
  m_UniformLocationMap["specularLightMaterialLocation"] = specularLightMaterialLocation;
  m_UniformLocationMap["specularPower"] = specularPowerLocation;
  m_UniformLocationMap["cameraPosition"] = cameraPositionLocation;
  m_UniformLocationMap["Model"] = modelLocation;
  m_UniformLocationMap["texture0"] = texture0Location;
  
}

void Material::loadDiffuseMap(const string& filename)
{
  m_DiffuseMap = loadTextureFromFile(filename);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glGenerateMipmap(GL_TEXTURE_2D);
}
void Material::LoadSpecularMap(const string& filename)
{
  m_SpecularMap = loadTextureFromFile(filename);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glGenerateMipmap(GL_TEXTURE_2D);
}

