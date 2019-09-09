/*  $Id$
 *
 *  Copyright (C) 2019 Zach Kanzler <they4kman@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __AIRHORN_H__
#define __AIRHORN_H__

G_BEGIN_DECLS


// Event ID to use with libcanberra, so airhorn sounds can be canceled.
#define CA_AIRHORN_ID 1


/* plugin structure */
typedef struct
{
    XfcePanelPlugin *plugin;

    /* panel widgets */
    GtkWidget       *ebox;
    GtkWidget       *hvbox;
    GtkWidget       *button;

    /* libcanberra context */
    ca_context      *ca_ctx;
}
AirhornPlugin;



void
airhorn_save (XfcePanelPlugin *plugin,
              AirhornPlugin   *airhorn);

G_END_DECLS

#endif /* !__AIRHORN_H__ */
