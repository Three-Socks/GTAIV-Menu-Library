/**
 * This file is from Three-Socks Menu Library https://bitbucket.org/ThreeSocks/gtaiv-menu-library
 *
 * GTAIV Menu Library
 *
 * @package menu
 * @author Three-Socks http://psx-scene.com/forums/members/three-socks/
 * @license LICENSE.txt DON'T BE A DICK PUBLIC LICENSE (DBAD)
 *
 * @version 1.3
 */

#pragma once

/*

Style: 1

Name: Movable menu with compact text and highlight box.

Desc: Mimics GTA Frontend.

Notes:
- all positioning is calculated from a base position allowing easy movement
- menu text is more compact and allows underscores
- right side items are set to right justify to slim down code a bit (added 2 vars to set_up_draw() to handle positioning)
- correct spacing for floats up to 9999 and up to 8 decimal spaces, and for integers up to 99999999
- changing text width, height, or spacing may still break some placements
- I'm probably forgetting some things...

Author: jumper http://psx-scene.com/forums/members/jumper/

*/

/**************** definitions ****************/

/* top left base position */
//#define menu_pos_x 0.0300
//#define menu_pos_y 0.0600

/* middle right base position */
#define menu_pos_x 0.7700
#define menu_pos_y 0.1890

/* text width */
#define menu_txt_w 0.2500

/* text height */
#define menu_txt_h 0.3500

/* text spacing (vertical) */
#define menu_txt_s 0.0280

/* menu colors - you can comment these out   */
/* to allow your menu to set / change them   */
#define menu_h_r 255
#define menu_h_g 128
#define menu_h_b 1

/*********************************************/

void style_setup(void)
{
	// Start y positioning.
	menu_start_y = menu_pos_y;
	// Spacing between each item.
	menu_spacing = menu_txt_s;
	// Max number of items before scrolling.
	menu_max = 22;
	// When to start scrolling.
	menu_start_scrolling = 11;
}

void draw_startup(void)
{
	SET_CINEMATIC_BUTTON_ENABLED(false);

	#ifdef PC
	if (!IS_FONT_LOADED(7))
		LOAD_TEXT_FONT(7);
	#else
	if (!IS_FONT_LOADED(6))
		LOAD_TEXT_FONT(6);
	#endif
	LOAD_TEXT_FONT(0);
	// Mimic cell phone.
	WAIT(500);
	CREATE_MOBILE_PHONE(2);

	DISABLE_FRONTEND_RADIO();

	BLOCK_PED_WEAPON_SWITCHING(GetPlayerPed(), true);

	SET_PLAYER_CAN_DO_DRIVE_BY(GetPlayerIndex(), false);

	REQUEST_STREAMED_TXD("network", 0);
	while (!HAS_STREAMED_TXD_LOADED("network"))
		WAIT(0);

	arrow_txd = GET_TEXTURE_FROM_STREAMED_TXD("network", "ICON_W_ARROW_UP");
	rightarrow_txd = GET_TEXTURE_FROM_STREAMED_TXD("network", "ICON_W_ARROW_RIGHT");

	PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_READY");
}

void draw_shutdown(void)
{
	RELEASE_TEXTURE(arrow_txd);
	RELEASE_TEXTURE(rightarrow_txd);
	MARK_STREAMED_TXD_AS_NO_LONGER_NEEDED("network");

	ENABLE_FRONTEND_RADIO();

	BLOCK_PED_WEAPON_SWITCHING(GetPlayerPed(), false);

	SET_PLAYER_CAN_DO_DRIVE_BY(GetPlayerIndex(), true);

	SET_CINEMATIC_BUTTON_ENABLED(true);

	DESTROY_MOBILE_PHONE();

	PLAY_AUDIO_EVENT("FRONTEND_MENU_MP_UNREADY");
}

void set_up_draw(float width, float height, uint r, uint g, uint b, uint a, bool isToggle, bool isNumber)
{
	SET_TEXT_FONT(0);
	SET_TEXT_BACKGROUND(0);
	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 255);
	SET_TEXT_EDGE(1, 0, 0, 0, 255);

	SET_TEXT_COLOUR(r, g, b, a);
	SET_TEXT_SCALE(width, height);
	SET_TEXT_PROPORTIONAL(1);

	SET_TEXT_JUSTIFY(0);
	SET_TEXT_CENTRE(0);
	if (isToggle)
	{
		SET_TEXT_RIGHT_JUSTIFY(1);
		SET_TEXT_WRAP(0.0000, menu_pos_x + 0.2040);
	}
	else if (isNumber)
	{
		SET_TEXT_RIGHT_JUSTIFY(1);
		SET_TEXT_WRAP(0.0000, menu_pos_x + 0.1920);
	}
	else
	{
		SET_TEXT_RIGHT_JUSTIFY(0);
	}
	SET_TEXT_USE_UNDERSCORE(1);

}

void drawHeader()
{
	float x, y, x2, y2;

	x = menu_pos_x - 0.0723;
	y = menu_pos_y - 0.0050;

#ifdef PC
	SET_TEXT_FONT(7);
#else
	SET_TEXT_FONT(6);
#endif
	SET_TEXT_BACKGROUND(0);
	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 255);
	SET_TEXT_EDGE(1, 0, 0, 0, 255);

	SET_TEXT_COLOUR(menu_h_r, menu_h_g, menu_h_b, 220);
	SET_TEXT_SCALE(0.3500, 0.4775);
	SET_TEXT_PROPORTIONAL(1);

	SET_TEXT_JUSTIFY(0);
	SET_TEXT_CENTRE(0);
	SET_TEXT_RIGHT_JUSTIFY(0);
	SET_TEXT_USE_UNDERSCORE(1);
	DISPLAY_TEXT_WITH_LITERAL_STRING(menu_pos_x, menu_pos_y - 0.0180, "STRING", menu_header);
	
	DRAW_RECT(x + 0.1750, y + 0.02600000, 0.21656250, 0.00577779, 0, 0, 0, 255);
	DRAW_RECT(x + 0.1750, y + 0.02600000, 0.21500000, 0.00300000, menu_h_r, menu_h_g, menu_h_b, 220);
}

void drawWindow(void)
{
	float x, y, x2, y2;

	HIDE_HELP_TEXT_THIS_FRAME();

	x = menu_pos_x - 0.0126;
	y = menu_pos_y - 0.0370;

	if (menu_len > menu_consts_max)
		y2 = 0.09 + (menu_consts_max * menu_txt_s);
	else
		y2 = 0.09 + (menu_len * menu_txt_s);

	DRAW_CURVED_WINDOW(x, y, 0.2300, y2, 245);
}

void drawFrontend(void)
{
	INIT_FRONTEND_HELPER_TEXT();
	if (menu_level == 1)
		DRAW_FRONTEND_HELPER_TEXT("LEAVE", "INPUT_F_CANCEL", 0);
	else
		DRAW_FRONTEND_HELPER_TEXT("BACK", "INPUT_F_CANCEL", 0);
	
	if (menu_item[item_highlighted].action)
		DRAW_FRONTEND_HELPER_TEXT("CONFIRM", "INPUT_F_ACCEPT", 0);
	else
		DRAW_FRONTEND_HELPER_TEXT("KYB_ENTER", "INPUT_F_ACCEPT", 0);

	if (menu_item[item_highlighted].type == 1 || menu_item[item_highlighted].type == 2)
		DRAW_FRONTEND_HELPER_TEXT("SCROLL", "PAD_DPAD_LR", 0);
}

// Draws the menu.
void menu_draw(void)
{
	uint d_r = 255, d_g = 255, d_b = 255;

	float pos_x = menu_pos_x, 

	menu_width = menu_txt_w, menu_height = menu_txt_h;

	uint r, g, b, a = 255;
	int I;
	float Ipos_y = menu_start_y;
	
	/* highlight box */
	if (!inError)
	{
		if (menu_len > menu_consts_max && item_highlighted >= menu_start_scrolling)
		{
			DRAW_RECT(menu_pos_x + 0.10250000, menu_pos_y + 0.01100000 + (menu_spacing * menu_start_scrolling), 0.21500000, 0.02550000, menu_h_r, menu_h_g, menu_h_b, 120);
		}
		else
		{
			DRAW_RECT(menu_pos_x + 0.10250000, menu_pos_y + 0.01100000 + (menu_spacing * item_highlighted), 0.21500000, 0.02550000, menu_h_r, menu_h_g, menu_h_b, 120);
		}
	}

	for (I = 1; I <= menu_len; I++)
	{
		Ipos_y = Ipos_y + menu_spacing;
		if (I <= menu_max && Ipos_y > (menu_consts_start_y + 0.0100))
		{
			r = d_r;
			g = d_g;
			b = d_b;

			/* highlighted text */
			/*if (item_highlighted == I && !inError)
			{
				r =  menu_h_r;
				g =  menu_h_g;
				b =  menu_h_b;
			}*/

			set_up_draw(menu_width, menu_height, r, g, b, a, false, false);

			if (menu_item[I].type == 5)
				DISPLAY_TEXT_WITH_LITERAL_STRING(pos_x, Ipos_y, "STRING", GET_STRING_FROM_TEXT_FILE(GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(menu_item[I].num_val)));
			else
				DISPLAY_TEXT_WITH_LITERAL_STRING(pos_x, Ipos_y, "STRING", menu_item[I].name);

			if (menu_item[I].type == 1 || menu_item[I].type == 2)
			{
				/* main_pos_x and toggle_pos_x are no longer used since we are using SET_TEXT_RIGHT_JUSTIFY(1) */
				float left_pos_x;
				uint dp;

				if (menu_item[I].type == 2)
				{
					if (custom_float_dp != 0)
						dp = custom_float_dp;
					else
						dp = 1;

					if (menu_item[I].float_val < 10.0)
					{
						left_pos_x = menu_pos_x + 0.1680 - (0.0095 * dp);
					}
					if (menu_item[I].float_val >= 10.0)
					{
						left_pos_x = menu_pos_x + 0.1585 - (0.0095 * dp);
					}
					if (menu_item[I].float_val >= 100.0)
					{
						left_pos_x = menu_pos_x + 0.1490 - (0.0095 * dp);
					}
					if (menu_item[I].float_val >= 1000.0)
					{
						left_pos_x = menu_pos_x + 0.1395 - (0.0095 * dp);
					}
				}
				else
				{
					if (menu_item[I].num_val < 10)
					{
						left_pos_x = menu_pos_x + 0.1724;
					}

					if (menu_item[I].num_val >= 10)
					{
						left_pos_x = menu_pos_x + 0.1629;
					}

					if (menu_item[I].num_val >= 100)
					{
						left_pos_x = menu_pos_x + 0.1534;
					}

					if (menu_item[I].num_val >= 1000)
					{
						left_pos_x = menu_pos_x + 0.1439;
					}

					if (menu_item[I].num_val >= 10000)
					{
						left_pos_x = menu_pos_x + 0.1344;
					}

					if (menu_item[I].num_val >= 100000)
					{
						left_pos_x = menu_pos_x + 0.1249;
					}

					if (menu_item[I].num_val >= 1000000)
					{
						left_pos_x = menu_pos_x + 0.1154;
					}

					if (menu_item[I].num_val >= 10000000)
					{
						left_pos_x = menu_pos_x + 0.1059;
					}
				}

				DRAW_SPRITE(rightarrow_txd, left_pos_x, Ipos_y + 0.0100, 0.0160, 0.0160, 180.0000, r, g, b, a);
				set_up_draw(menu_width, menu_height, r, g, b, a, false, true);
				if (menu_item[I].type == 1)
					DISPLAY_TEXT_WITH_NUMBER(0.0000, Ipos_y, "NUMBR", menu_item[I].num_val);
				else if (menu_item[I].type == 2)
					DISPLAY_TEXT_WITH_FLOAT(0.0000, Ipos_y, "NUMBR", menu_item[I].float_val, dp);
				DRAW_SPRITE(rightarrow_txd, menu_pos_x + 0.2024, Ipos_y + 0.0100 , 0.0160, 0.0160, 0.0000, r, g, b, a);
			}
			else if (menu_item[I].type == 3)
			{
				if (menu_item[I].extra_val)
				{
					set_up_draw(menu_width, menu_height, menu_h_r, menu_h_g, menu_h_b, a, true, false);
					char *txt_on;
					if (!IS_STRING_NULL(custom_bool_on))
					{
						txt_on = custom_bool_on;
					}
					else
						txt_on = menu_on;
					DISPLAY_TEXT_WITH_LITERAL_STRING(0.0000, Ipos_y, "STRING", txt_on);
				}
				else
				{
					set_up_draw(menu_width, menu_height, d_r, d_g, d_b, a, true, false);
					char *txt_off;
					if (!IS_STRING_NULL(custom_bool_off))
					{
						txt_off = custom_bool_off;
					}
					else
						txt_off = menu_off;
					DISPLAY_TEXT_WITH_LITERAL_STRING(0.0000, Ipos_y, "STRING", txt_off);
				}
			}
		}
	}

	if (menu_len > menu_consts_max && item_highlighted > menu_start_scrolling)
		DRAW_SPRITE(arrow_txd, menu_pos_x + 0.2074, menu_pos_y + 0.0370, 0.0160, 0.0160, 0, 255, 255, 255, 255);

	if (menu_len > menu_consts_max)
		DRAW_SPRITE(arrow_txd, menu_pos_x + 0.2074, menu_pos_y + 0.0370 + (menu_consts_max * menu_spacing), 0.0160, 0.0160, 180.0000, 255, 255, 255, 255);
}