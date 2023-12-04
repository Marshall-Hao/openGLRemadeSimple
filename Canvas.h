#pragma once
#include "GTMATH.hpp"
#include "Image.h"

#include <string.h>
#include <vector>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GT
{

	struct Point
	{
	public:
		float		m_x;
		float		m_y;
		float		m_z;
		RGBA	m_color;
		floatV2 m_uv;
		
		Point(float _x = 0, float _y = 0, float _z = 0,RGBA _color = RGBA(0, 0, 0, 0), floatV2 _uv = floatV2(0.0, 0.0))
		{
			m_x = _x;
			m_y = _y;
			m_z = _z;
			m_color = _color;
			m_uv = _uv;
		}
		~Point()
		{

		}
	};

	enum DATA_TYPE
	{
		GT_FLOAT = 0,
		GT_INT = 1,
	};

	enum DRAW_MODE
	{
		GT_LINE = 0,
		GT_TRIANGLE = 1,
	};
	struct DataElement
	{
		int m_size;
		DATA_TYPE m_type;
		int m_stride;
		unsigned char* m_data;
		DataElement()
		{
			m_size = -1;
			m_type = DATA_TYPE::GT_FLOAT;
			m_stride = -1;
			m_data = NULL;
		}

	};

	enum MatrixMode
	{	
		GT_MODEL = 0,
		GT_VIEW,
		GT_PROJECTION,
	};
	struct Statement
	{	
		unsigned char m_alphaLimit;
		bool m_useBlend;
		bool m_enabletEXTURE;
		const Image* m_texture;
		Image::TEXTURE_TYPE m_textureType;

		DataElement m_vertexData;
		DataElement m_colorData;
		DataElement m_textureData;

		MatrixMode m_matrixMode;
		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		std::vector<glm::mat4>  m_matrixVec;

		Statement()
		{	
			m_alphaLimit = 0;
			m_useBlend = false;
			m_enabletEXTURE = false;
			m_texture = nullptr;
			m_textureType = Image::TEXTURE_TYPE::TX_REPEAT;

			m_matrixMode = MatrixMode::GT_MODEL;
			m_modelMatrix = glm::mat4(1.0f);
			m_viewMatrix = glm::mat4(1.0f);
			m_projectionMatrix = glm::mat4(1.0f);

			m_matrixVec.clear();
		}
	};

	class Canvas
	{
	private:
		int m_width;
		int m_height;
		RGBA* m_buffer;
		float* m_zBuffer;

		Statement m_state;
	public:
		Canvas(int _width, int _height, void* _buffer)
		{
			if (_width <= 0 || _height <= 0)
			{
				m_width = -1;
				m_height = -1;
				m_buffer = nullptr;
			}

			m_width =  _width;
			m_height = _height;
			m_buffer = (RGBA*)_buffer;
			m_zBuffer = new float[_width * _height];
		}
		~Canvas()
		{
			delete [] m_zBuffer;
		}
		inline RGBA colorLerp(RGBA _color1, RGBA _color2, float _scale)
		{
			RGBA	_color;
			_color.m_r = _color1.m_r + (float)(_color2.m_r - _color1.m_r) * _scale;
			_color.m_g = _color1.m_g + (float)(_color2.m_g - _color1.m_g) * _scale;
			_color.m_b = _color1.m_b + (float)(_color2.m_b - _color1.m_b) * _scale;
			_color.m_a = _color1.m_a + (float)(_color2.m_a - _color1.m_a) * _scale;
			return _color;
		}

		inline floatV2 uvLerp(floatV2 _uv1, floatV2 _uv2, float _scale)
		{
			floatV2	_uv;
			_uv.x = _uv1.x + (_uv2.x - _uv1.x) * _scale;
			_uv.y = _uv1.y + (_uv2.y - _uv1.y) * _scale;
			return _uv;
		}

		inline float zLerp(float _z1, float _z2, float _scale)
		{
	
			return _z1 + (_z2 - _z1) * _scale;
		}

		void clear()
		{
			if (m_buffer != NULL)
			{
				memset(m_buffer, 0, m_width * m_height * sizeof(RGBA));
				memset(m_zBuffer, 255, m_width * m_height * sizeof(float));
			}
		}
		// draw points	
		void drawPoint(Point _pt)
		{
			if (_pt.m_x < 0 || _pt.m_x >= m_width || _pt.m_y < 0 || _pt.m_y >= m_height)
			{
				return;
			}
			int _index = _pt.m_y * m_width + _pt.m_x;
			if (_pt.m_z > m_zBuffer[_index])
			{
				return;
			}

			m_zBuffer[_index] = _pt.m_z;
			m_buffer[_index]  = _pt.m_color;
		}

		RGBA getColor(int x, int y)
		{
			if (x < 0 || x >= m_width || y < 0 || y >= m_height)
			{
				return RGBA(0, 0, 0, 0);
			}

			return m_buffer[y * m_width + x];
		}

		
		void drawLine(Point pt1, Point pt2);
		/*void drawTriangle(Point pt1, Point pt2, Point pt3);	*/
		void drawTriangleFlat(Point ptFlat1, Point ptFlat2, Point pt);
		void drawTriangle(Point pt1, Point pt2, Point pt3);

		bool judgeInRectr(Point pt, GT_RECT _rect);
		bool judgeInTriangle(Point pt, std::vector<Point> _ptArray);
		bool judgeLineAndRect(int _x1, int _x2, int& _x1Cut, int& _x2Cut);
		// Image
		void drawImage(int _x, int _y,Image* _image);
		void setAlphaLimit(unsigned char _alphaLimit = 0) { m_state.m_alphaLimit = _alphaLimit; }
		void setBlend(bool _judge) { m_state.m_useBlend = _judge; }

		// uv
		void enableTexture(bool _judge) { m_state.m_enabletEXTURE = _judge; }
		// °ó¶¨Í¼Æ¬
		void bindTexture(const Image* _image) { m_state.m_texture = _image; }
		void setTextureType(Image::TEXTURE_TYPE _type) { m_state.m_textureType = _type; }
		
		// uv and color state
		void gtVertexPointer(int _size, DATA_TYPE _type, int _stride, unsigned char* _data);
		void gtColorPoionter(int _size, DATA_TYPE _type, int _stride, unsigned char* _data);
		void gtTexCoordPointer(int _size, DATA_TYPE _type, int _stride, unsigned char* _data);
		void gtDrawArray(DRAW_MODE _mode, int _first, int _count);

		// Matrix state
		void gtPushMatrix()
		{
			m_state.m_matrixVec.push_back(m_state.m_modelMatrix);
		}

		void gtPopMatrix()
		{
			if (m_state.m_matrixVec.size() > 0)
			{
				m_state.m_modelMatrix = m_state.m_matrixVec.back();
				m_state.m_matrixVec.pop_back();
			}
		}
		void gtMatrixMode(MatrixMode _mode)
		{
			m_state.m_matrixMode = _mode;
		}

		void gtLoadMatrix(glm::mat4 _matrix)
		{
			switch (m_state.m_matrixMode)
			{
			case MatrixMode::GT_MODEL:
			{
				m_state.m_modelMatrix = _matrix;
				break;
			}
			case MatrixMode::GT_VIEW:
			{
				m_state.m_viewMatrix = _matrix;
				break;
			}
			case MatrixMode::GT_PROJECTION:
			{
				m_state.m_projectionMatrix = _matrix;
				break;
			}
			default:
				break;
			}
		}

		void gtLoadIdentity()
		{
			switch (m_state.m_matrixMode)
			{
			case MatrixMode::GT_MODEL:
			{
				m_state.m_modelMatrix = glm::mat4(1.0f);
				break;
			}
			case MatrixMode::GT_VIEW:
			{
				m_state.m_viewMatrix = glm::mat4(1.0f);
				break;
			}
			case MatrixMode::GT_PROJECTION:
			{
				m_state.m_projectionMatrix = glm::mat4(1.0f);
				break;
			}
			default:
				break;
			}
		}

		void gtMulplyMatrix(glm::mat4 _matrix)
		{
			switch (m_state.m_matrixMode)
			{
			case MatrixMode::GT_MODEL:
			{
				m_state.m_modelMatrix = _matrix * m_state.m_modelMatrix;
				break;
			}
			case MatrixMode::GT_VIEW:
			{
				m_state.m_viewMatrix = _matrix * m_state.m_viewMatrix;
				break;
			}
			case MatrixMode::GT_PROJECTION:
			{
				m_state.m_projectionMatrix = _matrix * m_state.m_projectionMatrix ;
				break;
			}
			default:
				break;
			}
		}

		void gtVertexTransform(Point& _pt)
		{
			glm::vec4 ptv4(_pt.m_x, _pt.m_y, _pt.m_z, 1.0f);
			ptv4 = m_state.m_modelMatrix * ptv4;
			ptv4 = m_state.m_viewMatrix * ptv4;
			ptv4 = m_state.m_projectionMatrix * ptv4;

			_pt.m_x = (ptv4.x / ptv4.w + 1.0) * (float)m_width / 2.0;;
			_pt.m_y = (ptv4.y / ptv4.w + 1.0) * (float)m_height / 2.0;
			_pt.m_z = ptv4.z / ptv4.w;
		}
;	};

}

