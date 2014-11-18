#include "stdafx.h"

#include "streamviewer.h"
#include "../common/Console.h"

StreamViewer::StreamViewer(Drawer& drawer):
m_frameQueue(max_framequeue_size),
m_drawer(drawer),
m_decoder(m_frameQueue, drawer)
{

}

StreamViewer::~StreamViewer()
{
	stop();
}

void StreamViewer::start()
{
	m_decoder.start(); // decoder should start before reader
}

void StreamViewer::stop()
{
	m_decoder.stop(); 
}
