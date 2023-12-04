#pragma once
#include "GTMATH.hpp"
#include <string>


namespace GT
{
	class Image
	{
	private:
		int m_width;
		int m_height;
		RGBA* m_data;

		float m_alpha;
		
	public:
		enum TEXTURE_TYPE
		{
			TX_CLAMP_TO_EDGE = 0,
			TX_REPEAT = 1,
		};

	public:
		Image(int _width, int height = 0, unsigned char* _data = NULL)
		{
			m_width = _width;
			m_height = height;
			if (_data)
			{
				m_data = new RGBA[m_width * m_height];
				memcpy(m_data, _data, sizeof(RGBA) * m_width * m_height);
			}
			m_alpha = 1.0f;
		
		}
		~Image()
		{
			if (m_data)
			{
				delete[] m_data;

			}
		}
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		inline float getAlpha() const
		{
			return m_alpha;
		}
		inline void setAlpha(float _alpha)
		{
			m_alpha = _alpha;
		}	
		RGBA getColor(int x, int y)const
		{
			if (x < 0 || x > m_width - 1 || y <0 || y > m_height - 1)
			{
				return RGBA(0, 0, 0, 0);
			}
			return m_data[y * m_width + x];
		}

		RGBA getColorByUV(float u, float v, TEXTURE_TYPE _type)const
		{	
			// ∞—uv”≥…‰µΩÕº∆¨…œ
			int x = u * m_width;
			int y = v * m_height;
			switch(_type)
			{
				case TX_CLAMP_TO_EDGE:
				{
					x = x < m_width ? x : (m_width - 1);
					y = y < m_height ? y : (m_height - 1);
				}
				break;
				case TX_REPEAT:
				{
					x = x % m_width;
					y = y % m_height;
				}
				break;
				default:
				break;
			}

			/*x = x % m_width;
			y = y % m_height;*/

		
			return getColor(x, y);
		}
	public:
		static Image* readFromFile(const char* _fileName);
		static Image* zoomImageSimple(const Image* _image, float _zoomX, float _zoomY);
		static Image* zoomImage(const Image* _image, float _zoomX, float _zoomY);
	};


}
