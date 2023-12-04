#include "image.h"
#include "GTMATH.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace GT 
{
	Image* Image::readFromFile(const char* _fileName)
	{
		int _picType = 0;
		int _width = 0;
		int _height = 0;

		// stbimage 读取图片是反过来的
		stbi_set_flip_vertically_on_load(true);
		unsigned char* bits = stbi_load(_fileName, &_width, &_height, &_picType, STBI_rgb_alpha);

		if (!bits) {
			// Handle error: Failed to load image
			return nullptr;
		}

		for (int i = 0;i < _width * _height * 4; i+=4)
		{
			unsigned char tmp = bits[i];
			bits[i] = bits[i + 2];
			bits[i + 2] = tmp;
		}
		Image* _image = new Image(_width, _height, bits);
		stbi_image_free(bits);
		return _image;
	}


	Image* Image::zoomImageSimple(const Image* _image, float _zoomX, float _zoomY)
	{
		int width = _image->getWidth() * _zoomX;
		int height = _image->getHeight() * _zoomY;
		unsigned char* data = new unsigned char[width * height * 4];
		Image* _resultImage = NULL;

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{	

				// 映射原图的坐标， 每个点倒推回去
				int _imageX = i / _zoomX;
				int _imageY = j / _zoomY;

				// 不要越界
				_imageX = _imageX < _image->getWidth() ? _imageX : (_image->getWidth() -1) ;
				_imageY = _imageY < _image->getHeight() ? _imageY : (_image->getHeight() - 1);

				RGBA _color = _image->getColor(_imageX, _imageY);
				memcpy(data + (j * width + i) * 4, &_color, sizeof(RGBA));	
			}
		}

		_resultImage = new Image(width, height, data);

		delete[] data;
		return _resultImage;
	}

	// Helper function to clamp color values
	int clamp(int value, int min, int max) {
		return (value < min) ? min : (value > max) ? max : value;
	}
	Image* Image::zoomImage(const Image* _image, float _zoomX, float _zoomY)
	{
		int _width = static_cast<int>(_image->getWidth() * _zoomX);
		int _height = static_cast<int>(_image->getHeight() * _zoomY);
		unsigned char* data = new unsigned char[_width * _height * 4];

		for (int i = 0; i < _width; i++)
		{
			float coordX = static_cast<float>(i) / _zoomX;
			int x1 = static_cast<int>(coordX);
			int x2 = (x1 < _image->getWidth() - 1) ? x1 + 1 : x1;
			float disX1 = coordX - x1;
			float disX2 = 1.0f - disX1;

			for (int j = 0; j < _height; j++)
			{
				float coordY = static_cast<float>(j) / _zoomY;
				int y1 = static_cast<int>(coordY);
				int y2 = (y1 < _image->getHeight() - 1) ? y1 + 1 : y1;
				float disY1 = coordY - y1;
				float disY2 = 1.0f - disY1;

				RGBA _color11 = _image->getColor(x1, y1);
				RGBA _color21 = _image->getColor(x2, y1);
				RGBA _color12 = _image->getColor(x1, y2);
				RGBA _color22 = _image->getColor(x2, y2);

				RGBA _targetColor;
				// Ensure color values are clamped between 0 and 255
				_targetColor.m_r = clamp((_color11.m_r * disX2 + _color21.m_r * disX1) * disY2 + (_color12.m_r * disX2 + _color22.m_r * disX1) * disY1, 0, 255);
				_targetColor.m_g = clamp((_color11.m_g * disX2 + _color21.m_g * disX1) * disY2 + (_color12.m_g * disX2 + _color22.m_g * disX1) * disY1, 0, 255);
				_targetColor.m_b = clamp((_color11.m_b * disX2 + _color21.m_b * disX1) * disY2 + (_color12.m_b * disX2 + _color22.m_b * disX1) * disY1, 0, 255);
				_targetColor.m_a = clamp((_color11.m_a * disX2 + _color21.m_a * disX1) * disY2 + (_color12.m_a * disX2 + _color22.m_a * disX1) * disY1, 0, 255);

				memcpy(data + (j * _width + i) * 4, &_targetColor, sizeof(RGBA));
			}
		}

		Image* _resultImage = new Image(_width, _height, data);
		delete[] data; // If Image constructor copies data, else remove this line
		return _resultImage;
	}

	
}


