#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_IMPLEMENTATION
#include "nuklear_blend2d.h"
#include <iostream>
using std::cerr;

static float nk_blend2d_font_get_width(nk_handle ptr, float height, const char* text, int length)
{
    BLFont* font = (BLFont*) ptr.ptr;

    BLGlyphBuffer gb;
    BLTextMetrics tm;

    if(font->empty())
    {
        cerr << "ERROR: Invalid font passed to nk_blend2d_font_get_width()\n";
    }

    gb.setUtf8Text(text, length);
    font->shape(gb);
    font->getTextMetrics(gb, tm);
    return (tm.boundingBox.x1 - tm.boundingBox.x0);
}

NK_API struct nk_context nk_blend2d_init(BLFont& font)
{
    static struct nk_context ctx;

    static struct nk_user_font nfont;
    nfont.userdata = nk_handle_ptr(&font);
    nfont.height = font.metrics().ascent + font.metrics().descent;
    nfont.width = nk_blend2d_font_get_width;

    if(!nk_init_default(&ctx, &nfont))
    {
        cerr << "ERROR: Failed to initialize Nuklear context";
    }

    return ctx;
}


NK_API void nk_blend2d_render(struct nk_context* nctx, BLContext& bctx)
{
    const struct nk_command* cmd = 0;
    bctx.clearAll();
    nk_foreach(cmd, nctx)
    {
        switch(cmd->type)
        {
        case NK_COMMAND_NOP:
            break;

        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor* data =(const struct nk_command_scissor*)cmd;
            // Do not enable! It causes some wild clipping issues...
            // bctx.clipToRect((double)data->x, (double)data->y, (double)data->w, (double)data->h);
        } break;

        case NK_COMMAND_LINE: {
            const struct nk_command_line* data = (const struct nk_command_line*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.strokeLine(data->begin.x, data->begin.y, data->end.x, data->end.y);
        } break;

        case NK_COMMAND_RECT: {
            const struct nk_command_rect* data = (const struct nk_command_rect*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.strokeRoundRect((double)data->x, (double)data->y, (double)data->w, (double)data->h, (double)data->rounding);
        } break;

        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled* data = (const struct nk_command_rect_filled*)cmd;
            bctx.setFillStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.fillRoundRect((double)data->x, (double)data->y, (double)data->w, (double)data->h, (double)data->rounding);
        } break;

        case NK_COMMAND_CIRCLE: {
            const struct nk_command_circle* data = (const struct nk_command_circle*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.strokeRoundRect((double)data->x, (double)data->y, (double)data->w, (double)data->h, (double)(data->w / 2));
        } break;

        case NK_COMMAND_CIRCLE_FILLED: {
            const struct nk_command_circle_filled* data = (const struct nk_command_circle_filled*)cmd;
            bctx.setFillStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.fillRoundRect((double)data->x, (double)data->y, (double)data->w, (double)data->h, (double)(data->w / 2));
        } break;

        case NK_COMMAND_ARC: {
            const struct nk_command_arc* data = (const struct nk_command_arc*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.strokeArc((double)data->cx, (double)data->cy, (double)data->r, (double)data->a[0], (double)data->a[1]);
        } break;

        case NK_COMMAND_ARC_FILLED: {
            const struct nk_command_arc_filled* data = (const struct nk_command_arc_filled*)cmd;
            bctx.setFillStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.fillPie((double)data->cx, (double)data->cy, (double)data->r, (double)data->a[0], (double)data->a[1]);
        } break;

        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle* data = (const struct nk_command_triangle*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.strokeTriangle((double)data->a.x, (double)data->a.y, (double)data->b.x, (double)data->b.y, (double)data->c.x, (double)data->c.y);
        } break;

        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled* data = (const struct nk_command_triangle_filled*)cmd;
            bctx.setFillStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));
            bctx.fillTriangle((double)data->a.x, (double)data->a.y, (double)data->b.x, (double)data->b.y, (double)data->c.x, (double)data->c.y);
        } break;

        case NK_COMMAND_POLYGON: {
            const struct nk_command_polygon* data =(const struct nk_command_polygon*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));

            BLPointI* points = (BLPointI*)malloc(sizeof(BLPointI) * data->point_count);
            for(int i = 0; i < data->point_count; i++)
            {
                points[i].x = data->points[i].x;
                points[i].y = data->points[i].y;
            }

            bctx.strokePolygon(points, data->point_count);
            free(points);
        } break;

        case NK_COMMAND_POLYGON_FILLED: {
            const struct nk_command_polygon_filled* data = (const struct nk_command_polygon_filled*)cmd;
            bctx.setFillStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));

            BLPointI* points = (BLPointI*)malloc(sizeof(BLPointI) * data->point_count);
            for(int i = 0; i < data->point_count; i++)
            {
                points[i].x = data->points[i].x;
                points[i].y = data->points[i].y;
            }

            bctx.fillPolygon(points, data->point_count);
            free(points);
        } break;

        case NK_COMMAND_POLYLINE: {
            const struct nk_command_polyline* data = (const struct nk_command_polyline*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));

            BLPointI* points = (BLPointI*)malloc(sizeof(BLPointI) * data->point_count);
            for(int i = 0; i < data->point_count; i++)
            {
                points[i].x = data->points[i].x;
                points[i].y = data->points[i].y;
            }

            bctx.strokePolyline(points, data->point_count);
            free(points);
        } break;

        case NK_COMMAND_TEXT: {
            const struct nk_command_text* data = (const struct nk_command_text*)cmd;
            bctx.setFillStyle(BLRgba32(data->foreground.r, data->foreground.g, data->foreground.b, data->foreground.a));
            BLFont* font = (BLFont*)data->font->userdata.ptr;
            bctx.fillUtf8Text(BLPoint(data->x, data->y + font->metrics().ascent), *font, (const char*)data->string, data->length);
        } break;

        case NK_COMMAND_CURVE: {
            const struct nk_command_curve* data = (const struct nk_command_curve*)cmd;
            bctx.setStrokeWidth((double)data->line_thickness);
            bctx.setStrokeStyle(BLRgba32(data->color.r, data->color.g, data->color.b, data->color.a));

            BLPath curve;
            curve.moveTo(BLPoint((double)data->begin.x, (double)data->begin.y));
            curve.cubicTo((double)data->ctrl[0].x, (double)data->ctrl[0].y,
                          (double)data->ctrl[1].x, (double)data->ctrl[1].y,
                          (double)data->end.x, (double)data->end.y);
        } break;

        case NK_COMMAND_RECT_MULTI_COLOR: {
            // TODO: implement multi colour rectangle rendering
            // Blend2D currently does not support per-vertex colouring
            // May have to write a custom software rasterizer
            
            // QuickFix: Draw the rectangle with the colour of first vertex
            // This command is currently only used by the colour picker
            // If you don't use it, you're good to go!

            const struct nk_command_rect_multi_color* data = (const struct nk_command_rect_multi_color*)cmd;
            bctx.setFillStyle(BLRgba32(data->left.r, data->left.g, data->left.b, data->left.a));
            bctx.fillRect(data->x, data->y, data->w, data->h);
        } break;

        case NK_COMMAND_IMAGE: {            
            const struct nk_command_image* data = (const struct nk_command_image*)cmd;
            BLImage img;
            img.createFromData(data->w, data->h, BL_FORMAT_PRGB32, data->img.handle.ptr, data->w, BL_DATA_ACCESS_READ);
            BLPattern pattern(img);
            bctx.fillRect(data->x, data->y, data->w, data->h, pattern);
        } break;

        case NK_COMMAND_CUSTOM:
        default: break;
        }
    }
}
