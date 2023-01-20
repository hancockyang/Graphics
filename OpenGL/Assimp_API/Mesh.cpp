#include "Mesh.hpp"

Mesh::Mesh(std::vector<glm::vec3> && vertexposition,
           std::vector<glm::vec2> && vertextexcoord,
           std::vector<glm::vec3> && vertexnormal,
           std::vector<int> && glDrawElements_index,
           GLuint diffusetexture)
    : m_vertexposition{std::move(vertexposition)}, m_vertextexcoord{std::move(vertextexcoord)},
      m_vertexnormal{std::move(vertexnormal)},
      m_glDrawElements_index{std::move(glDrawElements_index)}, 
      m_diffuseTexture{diffusetexture}, 
      m_vao{}, m_vbo{}, m_ebo{} {};

Mesh::~Mesh() = default;

void Mesh::BindData() {
  // create vertex array object
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  // create and initialize vertex buffer object
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  GLuint sizeofposition =
      static_cast<GLuint>(m_vertexposition.size() * sizeof(glm::vec3));
  GLuint sizeoftexture =
      static_cast<GLuint>(m_vertextexcoord.size() * sizeof(glm::vec2));
  GLuint sizeofnormal =
      static_cast<GLuint>(m_vertexnormal.size() * sizeof(glm::vec3));
  glBufferData(GL_ARRAY_BUFFER, sizeofposition + sizeoftexture + sizeofnormal,
               NULL, GL_STATIC_DRAW);

  // pass position
  GLuint offsetposition = 0;
  glBufferSubData(GL_ARRAY_BUFFER, offsetposition, sizeofposition,
                  m_vertexposition.data());
  glEnableVertexAttribArray(0); // in shader, layout = 0 for position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)offsetposition);

  // pass texture
  GLuint offsettexcoord = sizeofposition;
  glBufferSubData(GL_ARRAY_BUFFER, offsettexcoord, sizeoftexture,
                  m_vertextexcoord.data());
  glEnableVertexAttribArray(1); // in shader, layout = 1 for texture coordinate
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)offsettexcoord);

  // pass normal
  GLuint offsetnormal = sizeofnormal + sizeoftexture;
  glBufferSubData(GL_ARRAY_BUFFER, offsetnormal, sizeofnormal,
                  m_vertexnormal.data());
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)offsetnormal);

  // create EBO (element buffer object), to solve same vertex being used mutiple
  // time
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               m_glDrawElements_index.size() * sizeof(GLuint),
               m_glDrawElements_index.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void Mesh::Draw(GLuint program) const {
  glBindVertexArray(m_vao);

  // pass in texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);
  glUniform1i(glGetUniformLocation(program, "texture"), 0);

  // draw
  glDrawElements(GL_TRIANGLES,
                 static_cast<GLsizei>(m_glDrawElements_index.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
