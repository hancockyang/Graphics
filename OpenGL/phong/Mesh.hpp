#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
class Mesh {
public:
  Mesh(std::vector<glm::vec3> &&, std::vector<glm::vec2> &&,
       std::vector<glm::vec3> &&, std::vector<int> &&, GLuint);
  ~Mesh();
  void BindData();
  void Draw(GLuint program) const;

private:

  std::vector<glm::vec3> m_vertexposition;
  std::vector<glm::vec2> m_vertextexcoord;
  std::vector<glm::vec3> m_vertexnormal;
  std::vector<int> m_glDrawElements_index;
  GLuint m_diffuseTexture;

  GLuint m_vao, m_vbo, m_ebo;

};
