#ifndef colorsoace_h_858
#define colorsoace_h_858

class ColorSpaceDll
{
public:
	typedef void (__stdcall *dll_yuv12_2_rgb24)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * y_src,
		unsigned char * v_src,
		unsigned char * u_src,
		int y_stride,
		int uv_stride,
		int width,
		int height

		);

	typedef void (__stdcall *dll_yuv12_2_rgb32)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * y_src,
		unsigned char * v_src,
		unsigned char * u_src,
		int y_stride,
		int uv_stride,
		int width,
		int height

		);

	typedef void (__stdcall *dll_yuv12_2_rgb555)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * y_src,
		unsigned char * v_src,
		unsigned char * u_src,
		int y_stride,
		int uv_stride,
		int width,
		int height

		);

	typedef void (__stdcall *dll_yuv12_2_rgb565)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * y_src,
		unsigned char * v_src,
		unsigned char * u_src,
		int y_stride,
		int uv_stride,
		int width,
		int height

		);


	typedef void (__stdcall *dll_yuy2_2_rgb555)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * src,
		int src_stride,
		int width,
		int height

		);

	typedef void (__stdcall *dll_yuy2_2_rgb565)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * src,
		int src_stride,
		int width,
		int height

		);

	typedef void (__stdcall *dll_yuy2_2_rgb24)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * src,
		int src_stride,
		int width,
		int height

		);

	typedef void (__stdcall *dll_yuy2_2_rgb32)
		(
		unsigned char * dst,
		int dst_stride,
		unsigned char * src,
		int src_stride,
		int width,
		int height

		);


	dll_yuv12_2_rgb24 yuv12_2_rgb24;
	dll_yuv12_2_rgb32 yuv12_2_rgb32;
	dll_yuv12_2_rgb555 yuv12_2_rgb555;
	dll_yuv12_2_rgb555 yuv12_2_rgb565;

	dll_yuy2_2_rgb555 yuy2_2_rgb555;
	dll_yuy2_2_rgb565 yuy2_2_rgb565;
	dll_yuy2_2_rgb24 yuy2_2_rgb24;
	dll_yuy2_2_rgb32 yuy2_2_rgb32;

	static ColorSpaceDll& instance()
	{
		static ColorSpaceDll dll;
		return dll;
	}

	bool isinited() const 
	{
		return m_inited;
	}

private:

	ColorSpaceDll()
	{

		m_inited = true;

		const char* dll_name = "colorspacedll.dll";

		m_dll  = ::LoadLibrary(dll_name);

		if(!m_dll)
			m_inited = false;

		yuv12_2_rgb24 = reinterpret_cast<dll_yuv12_2_rgb24>(::GetProcAddress(m_dll, "yuv12_2_rgb24"));
		if(!yuv12_2_rgb24)
			m_inited = false;

		yuv12_2_rgb32 = reinterpret_cast<dll_yuv12_2_rgb32>(::GetProcAddress(m_dll, "yuv12_2_rgb32"));
		if(!yuv12_2_rgb32)
			m_inited = false;


		yuv12_2_rgb555 = reinterpret_cast<dll_yuv12_2_rgb555>(::GetProcAddress(m_dll, "yuv12_2_rgb555"));
		if(!yuv12_2_rgb555)
			m_inited = false;


		yuv12_2_rgb565 = reinterpret_cast<dll_yuv12_2_rgb565>(::GetProcAddress(m_dll, "yuv12_2_rgb565"));
		if(!yuv12_2_rgb565)
			m_inited = false;



		yuy2_2_rgb555 = reinterpret_cast<dll_yuy2_2_rgb555>(::GetProcAddress(m_dll, "yuy2_2_rgb555"));
		if(!yuv12_2_rgb565)
			m_inited = false;

		yuy2_2_rgb565 = reinterpret_cast<dll_yuy2_2_rgb565 >(::GetProcAddress(m_dll, "yuy2_2_rgb565"));
		if(!yuv12_2_rgb565)
			m_inited = false;

		yuy2_2_rgb24 = reinterpret_cast<dll_yuy2_2_rgb24 >(::GetProcAddress(m_dll, "yuy2_2_rgb24"));
		if(!yuy2_2_rgb24)
			m_inited = false;

		yuy2_2_rgb32 = reinterpret_cast<dll_yuy2_2_rgb32>(::GetProcAddress(m_dll, "yuy2_2_rgb32"));
		if(!yuy2_2_rgb24)
			m_inited = false;
	}

	~ColorSpaceDll()
	{
		::FreeLibrary(m_dll);
	}



private:
	HINSTANCE m_dll;
	bool m_inited;
};



#endif //colorsoace_h_858