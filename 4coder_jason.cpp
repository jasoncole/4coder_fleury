internal Range_i64
Jason_GetLineRange(Application_Links* app, View_ID view, Buffer_ID buffer)
{
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos = view_get_mark_pos(app, view);
    Range_i64 Result = Ii64(cursor_pos, mark_pos);
    Result = F4_LineRangeFromPosRange(app, buffer, Result);
    Result = F4_PosRangeFromLineRange(app, buffer, Result);
    Result.max += 1;
    return Result;
}

CUSTOM_COMMAND_SIG( jason_delete_line_range ) {
    View_ID view = get_active_view( app, 0 );
    Buffer_ID buffer = view_get_buffer( app, view, 0 );
    
    Range_i64 pos_range = Jason_GetLineRange(app, view, buffer);
    buffer_replace_range(app, buffer, pos_range, string_u8_empty);
}

CUSTOM_COMMAND_SIG( jason_copy_line_range ) {
    View_ID view = get_active_view( app, 0 );
    Buffer_ID buffer = view_get_buffer( app, view, 0 );
    
    Range_i64 pos_range = Jason_GetLineRange(app, view,  buffer);
    clipboard_post_buffer_range(app, 0, buffer, pos_range);
}

CUSTOM_COMMAND_SIG( jason_paste_new_line ) {
    View_ID view = get_active_view(app, 0);
    Buffer_ID buffer = view_get_buffer(app, view, 0);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 line = get_line_number_from_pos(app, buffer, cursor_pos);
    cursor_pos = get_line_end_pos(app, buffer, line);
    Buffer_Seek seek = seek_pos(cursor_pos);
    view_set_cursor(app, view, seek);
    // edit clipboard to insert newline character at start
}


