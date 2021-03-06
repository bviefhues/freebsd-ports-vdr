# Created by: Juergen Lock <nox@FreeBSD.org>
# $FreeBSD: head/multimedia/vdr-plugin-streamdev/Makefile 353191 2014-05-07 18:49:51Z nox $

PORTNAME=	vdr-plugin-streamdev
PORTVERSION=	0.6.1
CATEGORIES=	multimedia
MASTER_SITES=	http://projects.vdr-developer.org/attachments/download/1580/:release \
		LOCAL:snapshot
MASTER_SITE_SUBDIR=	nox/:snapshot
DISTFILES=	${DISTNAME:S/plugin-//}${EXTRACT_SUFX}:release

MAINTAINER=	nox@FreeBSD.org
COMMENT=	Video Disk Recorder - stream device plugin

RUN_DEPENDS+=	bash:${PORTSDIR}/shells/bash

USES=		tar:tgz
PORTDOCS=	COPYING README HISTORY
HAVE_CONFIGURE=	yes
MAKE_JOBS_UNSAFE=	yes
WRKSRC=		${WRKDIR}/${PLUGIN}-${PORTVERSION}
VDR_PLUGIN_MAKEFILES=	${WRKSRC}/client/Makefile \
		${WRKSRC}/server/Makefile

.include	"${.CURDIR}/../vdr/Makefile.plugins"

pre-patch:
	@${CP} ${WRKSRC}/Makefile-1.7.33 ${WRKSRC}/Makefile
	@${CP} ${WRKSRC}/client/Makefile-1.7.33 ${WRKSRC}/client/Makefile
	@${CP} ${WRKSRC}/server/Makefile-1.7.33 ${WRKSRC}/server/Makefile

post-patch: post-patch-plugin
	@${LN} -s ../locale ${WRKSRC}
	@${REINPLACE_CMD} '1s-^#!/bin/sh-#!${LOCALBASE}/bin/bash-' \
		${WRKSRC}/${PLUGIN}-server/externremux.sh

pre-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/lib/vdr

post-install: post-install-pluginlocales
	@${MKDIR} ${STAGEDIR}${PREFIX}/etc/vdr/plugins/${PLUGIN}-server
	@${INSTALL_SCRIPT} ${WRKSRC}/${PLUGIN}-server/externremux.sh \
		${STAGEDIR}${PREFIX}/etc/vdr/plugins/${PLUGIN}-server/externremux.sh.sample
	@${INSTALL_DATA} ${WRKSRC}/${PLUGIN}-server/streamdevhosts.conf \
		${STAGEDIR}${PREFIX}/etc/vdr/plugins/${PLUGIN}-server/streamdevhosts.conf.sample
	${MKDIR} ${STAGEDIR}${DOCSDIR}
	(cd ${WRKSRC} && ${INSTALL_DATA} ${PORTDOCS} ${STAGEDIR}${DOCSDIR})

.include <bsd.port.mk>
