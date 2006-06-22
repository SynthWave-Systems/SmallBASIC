/* -*- c-file-style: "java" -*-
 * $Id: form_ui.c,v 1.3 2006-06-22 12:10:57 zeeb90au Exp $
 * This file is part of SmallBASIC
 *
 * Copyright(C) 2001-2006 Chris Warren-Smith. Gawler, South Australia
 * cwarrens@twpo.com.au
 *
 * This program is distributed under the terms of the GPL v2.0 or later
 * Download the GNU Public License(GPL) from www.gnu.org
 */ 

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "sys.h"
#include "var.h"
#include "kw.h"
#include "pproc.h"
#include "device.h"
#include "smbas.h"
#include "blib_ui.h"

#include <gtk/gtk.h>
#include "output_model.h"

GtkWidget* form = 0; /* modal form */
extern OutputModel output;

typedef enum ControlType {
    ctrl_button,
    ctrl_radio,
    ctrl_check,
    ctrl_text,
    ctrl_label,
    ctrl_list
} ControlType;

typedef struct WidgetInfo {
    ControlType type;
    var_t* var;
} WidgetInfo;

WidgetInfo* get_widget_info(GtkWidget* w) {
    return (WidgetInfo*)g_object_get_data(G_OBJECT(w), "widget_info");
}

void set_widget_info(GtkWidget* w, WidgetInfo* inf) {
    g_object_set_data(G_OBJECT(w), "widget_info", inf);
}

gboolean button_cb(GtkWidget *widget, GdkEventButton *event) {
    WidgetInfo* inf = get_widget_info(widget);
    v_setstrn(inf->var, "1", 1);
    //wnd->setModal(false);
    //wnd->penState = 2;
}

void radio_cb(GtkWidget* widget, void* v) {
    WidgetInfo* inf = get_widget_info(widget);
    v_setstrn(inf->var, "1", 1);
}

// transfer widget data in variables
void update_vars(GtkWidget* widget) {
    WidgetInfo* inf = get_widget_info(widget);
    switch (inf->type) {
    case ctrl_check:
    case ctrl_radio:
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
            v_setstrn(inf->var, "1", 1);
        }
        break;
    case ctrl_text:
        // copy input data into variable
        gchar* p = gtk_entry_get_text(GTK_ENTRY(widget));
        if (p && p[0]) {
            v_setstrn(inf->var, p, strlen(p));
        }
        break;
    case ctrl_list:
        // copy drop list item into variable
        //v_setstr(inf->var, ((Choice*)w)->item()->label());
        break;
    default:
        break;
    }
}

void ui_begin() {
    if (form == 0) {
        form = gtk_layout_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER(output.widget), form);
        gtk_widget_show(form);
    }
}

void ui_reset() {
    if (form != 0) {
        GList* list = gtk_container_get_children(GTK_CONTAINER(form));
        int n = g_list_length(list);
        int i;
        for (i=0; i<n; i++) {
            GtkWidget* w = (GtkWidget*)g_list_nth_data(list, i);
            WidgetInfo* inf = get_widget_info(w);
            g_free(inf);
            gtk_container_remove(GTK_CONTAINER(form), w);
        }
        gtk_container_remove(GTK_CONTAINER(output.widget), form);
        g_list_free(list);
        form = 0;
    }
}

// BUTTON x, y, w, h, variable, caption [,type] 
//
// type can optionally be 'radio' | 'checkbox' | 'link' | 'choice'
// variable is set to 1 if a button or link was pressed (which 
// will have closed the form, or if a radio or checkbox was 
// selected when the form was closed
// 
void cmd_button() {
    int x, y, w, h;
    var_t* v = 0;
    char* caption = 0;
    char* type = 0;

    if (-1 != par_massget("IIIIPSs", &x, &y, &w, &h, &v, &caption, &type)) {
        GtkWidget* widget = 0;
        WidgetInfo* inf = (WidgetInfo*)g_malloc(sizeof(WidgetInfo));
        inf->var = v;

        ui_begin();
        if (type) {
            if (strncmp("radio", type, 5) == 0) {
                //widget = new RadioButton(x, y, w, h);
                //widget->callback(radio_cb);
                inf->type = ctrl_radio;
            } else if (strncmp("checkbox", type, 8) == 0) {
                //widget = new CheckButton(x, y, w, h);
                //widget->callback(radio_cb);
                inf->type = ctrl_check;
            } else if (strncmp("button", type, 6) == 0) {
                //widget = new Button(x, y, w, h);
                //widget->callback(radio_cb);
                inf->type = ctrl_button;
            } else if (strncmp("label", type, 8) == 0) {
                //widget = (Button*)new GtkWidget(x, y, w, h);
                inf->type = ctrl_label;
            } else if (strncmp("choice", type, 6) == 0) {
#if 0
                Choice* choice = new Choice(x, y, w, h);
                choice->begin();
                // "Easy|Medium|Hard"
                int itemIndex = 0;
                inf->type = ctrl_list;
                int len = caption ? strlen(caption) : 0;
                for (int i=0; i<len; i++) {
                    char* c = strchr(caption+i, '|');
                    int endIndex = c ? c-caption : len;
                    String s(caption+i, endIndex-i);
                    Item* item = new Item();
                    item->copy_label(s.toString());
                    i = endIndex;
                    if (v->type == V_STR && v->v.p.ptr &&
                        strcmp((const char*)v->v.p.ptr, s.toString()) == 0) {
                        choice->focus_index(itemIndex);
                    }
                    itemIndex++;
                }
                choice->user_data(inf);
                choice->end();
                pfree2(caption, type);
#endif
                return;
            } else {
                ui_reset();
                rt_raise("UI: UNKNOWN TYPE: %s", type);
            }
        }
        if (widget == 0) {
            //widget = new Button(x, y, w, h);
            //widget->callback(button_cb);
            //inf->type = ctrl_button;
        }

        // prime input field from variable
        if (v->type == V_STR && v->v.p.ptr &&
            strcmp((const char*)v->v.p.ptr, "1") == 0) {
            if (inf->type == ctrl_check || 
                inf->type == ctrl_radio) {
                //widget->value(true);
            } else if (inf->type != ctrl_button) {
                //widget->value((const char*)v->v.p.ptr);
            }
        }

        //widget->copy_label(caption);
        //widget->user_data(inf);
    }
    pfree2(caption, type);
}

// TEXT x, y, w, h, variable
// When DOFORM returns the variable contains the user entered value
//
void cmd_text() {
    int x, y, w, h;
    var_t* v = 0;

    if (-1 != par_massget("IIIIP", &x, &y, &w, &h, &v)) {
        ui_begin();
        GtkWidget* entry = gtk_entry_new();

        // prime field from var_t
        if (v->type == V_STR && v->v.p.ptr) {
            gtk_entry_set_text(GTK_ENTRY(entry), (const char*)v->v.p.ptr);
        }
    
        gtk_layout_put(GTK_LAYOUT(form), entry, x, y);
        gtk_entry_set_has_frame(GTK_ENTRY(entry), TRUE);
        gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
        gtk_widget_set_size_request(entry, w, h);
        gtk_widget_modify_font(entry, output.font_desc);
        gtk_widget_show(entry);
        
        GtkIMContext* imctx = gtk_im_multicontext_new();
        gtk_im_context_set_client_window(imctx, output.widget->window);
        gtk_im_context_focus_in(imctx);
        gtk_widget_grab_focus(entry);

#ifdef USE_HILDON
        g_object_set(G_OBJECT(entry), "autocap", FALSE, NULL);
        gtk_im_context_show(imctx);
#endif

        WidgetInfo* inf = (WidgetInfo*)g_malloc(sizeof(WidgetInfo));
        inf->var = v;
        inf->type = ctrl_text;
        set_widget_info(entry, inf);
    }
}

// DOFORM [x,y,w,h [,border-style, bg-color]]
// Executes the form
void cmd_doform() {
    int x, y, w, h;
    int num_args;

    if (form == 0) {
        rt_raise("UI: DOFORM called before TEXT or BUTTON.");
        return;
    }

    x = y = w = h = 0;
    num_args = par_massget("iiii", &x, &y, &w, &h);

    if (num_args != 4) {
        ui_reset();
        rt_raise("UI: INVALID FORM ARGUMENTS: %d", num_args);
        return;
    }
    
    if (x < 2) {
        x = 2;
    }
    if (y < 2) {
        y = 2;
    }
    if (x+w > output.width) {
        w = output.width-x;
    }
    if (y+h > output.height) {
        h = output.height-y;
    }

    gtk_layout_move(GTK_LAYOUT(output.widget), form, x, y);
    gtk_layout_set_size(GTK_LAYOUT(form), w, h);
    gtk_widget_set_size_request(GTK_WIDGET(form), w, h);
    gtk_widget_grab_focus(form);
    gtk_widget_show(form);

    output.modal_flag = TRUE;
    while (output.modal_flag && output.break_exec == 0) {
        gtk_main_iteration_do(TRUE);
    }

    GList* list = gtk_container_get_children(GTK_CONTAINER(form));
    int n = g_list_length(list);
    int i;
    for (i=0; i<n; i++) {
        update_vars((GtkWidget*)g_list_nth_data(list, i));
    }
    g_list_free(list);

    ui_reset();
}

/* End of "$Id: form_ui.c,v 1.3 2006-06-22 12:10:57 zeeb90au Exp $". */