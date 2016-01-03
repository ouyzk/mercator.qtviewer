#include "geographicsellipseitem.h"
#include "../qtviewer_planetosm/osmtiles/viewer_interface.h"
#include <assert.h>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
namespace QTVP_GEOMARKER{
	geoGraphicsEllipseItem::geoGraphicsEllipseItem(
			QString name,
			QTVOSM::viewer_interface * pVi,
			qreal cent_lat,
			qreal cent_lon,
			qreal pxwidth,
			qreal pxheight)
		:QGraphicsEllipseItem(0)
		,geoItemBase(name,ITEAMTYPE_ELLIPSE_POINT,pVi)
		,m_lat(cent_lat)
		,m_lon(cent_lon)
		,m_width(pxwidth)
		,m_height(pxheight)

	{
		assert(vi()!=0);
		double px,py;
		vi()->CV_LLA2World(m_lat,m_lon,&px,&py);
		setRect(px - pxwidth/2, py - pxheight/2, pxwidth, pxheight);
	}
	void geoGraphicsEllipseItem::adjust_coords(int nNewLevel)
	{
		if (vi() && nNewLevel != level())
		{
			double ratio = pow(2.0,(nNewLevel - level()));
			QRectF rect = this->rect();
			QPointF center = rect.center();
			setRect(center.x() * ratio - rect.width()/2,
					center.y() * ratio - rect.height()/2,
					rect.width(),
					rect.height());
		}
	}

	void geoGraphicsEllipseItem::setSize(qreal pxwidth,qreal pxheight)
	{
		double px,py;
		vi()->CV_LLA2World(m_lat,m_lon,&px,&py);
		m_width = pxwidth;
		m_height = pxheight;
		setRect(px - pxwidth/2, py - pxheight/2, pxwidth, pxheight);
		adjustLabelPos();
	}

	void geoGraphicsEllipseItem::setCenter(qreal cent_lat,qreal cent_lon)
	{
		m_lat = cent_lat;
		m_lon = cent_lon;
		double px,py;
		vi()->CV_LLA2World(cent_lat,cent_lon,&px,&py);
		setRect(px - m_width/2, py - m_height/2, m_width, m_height);
		adjustLabelPos();
	}
	void geoGraphicsEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
	{
		QGraphicsEllipseItem::mousePressEvent(event);
		bool bshow = this->props_visible();
		this->show_props(!bshow);
		//event->accept();
	}
	QPointF geoGraphicsEllipseItem::label_pos()
	{
		return QPointF(this->rect().right(),rect().top()-8);
	}
}