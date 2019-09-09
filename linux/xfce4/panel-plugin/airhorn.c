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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <canberra.h>

#include "airhorn.h"


/* prototypes */
static void
airhorn_construct(XfcePanelPlugin *plugin);

static gboolean
airhorn_play(GtkWidget *panel_button, GdkEventButton *event, AirhornPlugin *airhorn);


/* register the plugin */
XFCE_PANEL_PLUGIN_REGISTER (airhorn_construct);


static AirhornPlugin *
airhorn_new(XfcePanelPlugin *plugin) {
    AirhornPlugin *airhorn;
    GtkOrientation orientation;

    /* allocate memory for the plugin structure */
    airhorn = g_slice_new0(AirhornPlugin);

    /* pointer to plugin */
    airhorn->plugin = plugin;

    /* libcanberra context to play/cache sounds with */
    ca_context_create(&airhorn->ca_ctx);

    /* get the current orientation */
    orientation = xfce_panel_plugin_get_orientation(plugin);

    /* create some panel widgets */
    airhorn->ebox = gtk_event_box_new();
    gtk_widget_show(airhorn->ebox);

    airhorn->hvbox = gtk_box_new(orientation, 2);
    gtk_widget_show(airhorn->hvbox);
    gtk_container_add(GTK_CONTAINER(airhorn->ebox), airhorn->hvbox);

    /* the airhorn widget */
    airhorn->button = gtk_button_new_from_icon_name("airhorn", GTK_ICON_SIZE_DIALOG);
    gtk_button_set_relief(GTK_BUTTON(airhorn->button), GTK_RELIEF_NONE);

    g_signal_connect(G_OBJECT(airhorn->button), "button-press-event",
                     G_CALLBACK(airhorn_play), airhorn);

    gtk_widget_show(airhorn->button);
    gtk_box_pack_start(GTK_BOX(airhorn->hvbox), airhorn->button, FALSE, FALSE, 0);

    return airhorn;
}


static void
airhorn_free(XfcePanelPlugin *plugin,
             AirhornPlugin *airhorn) {
    /* destroy the panel widgets */
    gtk_widget_destroy(airhorn->hvbox);

    /* free the plugin structure */
    g_slice_free(AirhornPlugin, airhorn);
}


static void
airhorn_orientation_changed(XfcePanelPlugin *plugin,
                            GtkOrientation orientation,
                            AirhornPlugin *airhorn) {
    /* change the orienation of the box */
    gtk_orientable_set_orientation(GTK_ORIENTABLE(airhorn->hvbox), orientation);
}


static gboolean
airhorn_size_changed(XfcePanelPlugin *plugin,
                     gint size,
                     AirhornPlugin *airhorn) {
    GtkOrientation orientation;
    GdkPixbuf *icon;
    GtkWidget *image;
    GError *err = NULL;

    /* get the orientation of the plugin */
    orientation = xfce_panel_plugin_get_orientation(plugin);

    /* set the widget size */
    if (orientation == GTK_ORIENTATION_HORIZONTAL) {
        gtk_widget_set_size_request(GTK_WIDGET(plugin), -1, size);
    } else {
        gtk_widget_set_size_request(GTK_WIDGET(plugin), size, -1);
    }

    icon = gtk_icon_theme_load_icon(
            gtk_icon_theme_get_default(),
            "airhorn",
            size - 10,
            0,
            &err
            );

    if (err != NULL) {
        g_error("Error resizing icon: %s", err->message);
    } else {
        image = gtk_button_get_image(GTK_BUTTON(airhorn->button));
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), icon);
    }

    /* we handled the orientation */
    return TRUE;
}


static gboolean
airhorn_play(GtkWidget *panel_button,
             GdkEventButton *event,
             AirhornPlugin *airhorn) {

    switch (event->button) {
        // left-click
        case 1: {
            ca_context_play(airhorn->ca_ctx,
                            CA_AIRHORN_ID,
                            CA_PROP_EVENT_ID, "airhorn",
                            NULL);
            return TRUE;
        }

        // middle-click
        case 2: {
            ca_context_cancel(airhorn->ca_ctx,
                              CA_AIRHORN_ID);
            return TRUE;
        }

        default: {
            return FALSE;
        }
    }
}


static void
airhorn_construct(XfcePanelPlugin *plugin) {
    AirhornPlugin *airhorn;

    /* setup transation domain */
    xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

    /* create the plugin */
    airhorn = airhorn_new(plugin);

    /* cache airhorn sound */
    ca_context_cache(airhorn->ca_ctx,
                     CA_PROP_EVENT_ID, "airhorn",
                     NULL);

    /* add the ebox to the panel */
    gtk_container_add(GTK_CONTAINER(plugin), airhorn->ebox);

    /* show the panel's right-click menu on this ebox */
//    xfce_panel_plugin_add_action_widget(plugin, airhorn->ebox);

    /* connect plugin signals */
    g_signal_connect(G_OBJECT(plugin), "free-data",
                     G_CALLBACK(airhorn_free), airhorn);

    g_signal_connect(G_OBJECT(plugin), "size-changed",
                     G_CALLBACK(airhorn_size_changed), airhorn);

    g_signal_connect(G_OBJECT(plugin), "orientation-changed",
                     G_CALLBACK(airhorn_orientation_changed), airhorn);
}
