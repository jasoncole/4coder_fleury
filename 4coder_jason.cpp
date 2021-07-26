internal Range_i64
copy_F4_LineRangeFromPosRange(Application_Links *app, Buffer_ID buffer, Range_i64 pos_range)
{
    Range_i64 lines_range =
        Ii64(get_line_number_from_pos(app, buffer, pos_range.min),
             get_line_number_from_pos(app, buffer, pos_range.max));
    return lines_range;
}

internal Range_i64
copy_F4_PosRangeFromLineRange(Application_Links *app, Buffer_ID buffer, Range_i64 line_range)
{
    if(line_range.min > line_range.max)
    {
        i64 swap = line_range.max;
        line_range.max = line_range.min;
        line_range.min = swap;
    }
    Range_i64 pos_range =
        Ii64(get_line_start_pos(app, buffer, line_range.min),
             get_line_end_pos(app, buffer, line_range.max));
    return pos_range;
}


internal Range_i64
Jason_GetLineRange(Application_Links* app, View_ID view, Buffer_ID buffer)
{
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos = view_get_mark_pos(app, view);
    Range_i64 Result = Ii64(cursor_pos, mark_pos);
    Result = copy_F4_LineRangeFromPosRange(app, buffer, Result);
    Result = copy_F4_PosRangeFromLineRange(app, buffer, Result);
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

CUSTOM_COMMAND_SIG(jason_switch_to_normal)
{
    switch_to_keybinding_0(app);
    set_mode_to_original(app);
    global_hide_region_boundary = false;
}

CUSTOM_COMMAND_SIG(jason_switch_to_insert)
{
    switch_to_keybinding_1(app);
	global_hide_region_boundary = true;
}

CUSTOM_COMMAND_SIG(jason_switch_to_notepad)
{
    switch_to_keybinding_2(app);
    set_mode_to_notepad_like(app);
}

