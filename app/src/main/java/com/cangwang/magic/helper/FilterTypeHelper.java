package com.cangwang.magic.helper;


import com.cangwang.magic.R;

public class FilterTypeHelper {
	
	public static int FilterType2Color(int type){

		MagicFilterType filterType = MagicFilterType.values()[type];
		switch (filterType) {
			case NONE:
				return R.color.filter_color_grey_light;
			case WHITECAT:
			case BLACKCAT:
			case SUNRISE:
			case SUNSET:
				return R.color.filter_color_brown_light;
			case COOL:
				return R.color.filter_color_blue_dark;
			case EMERALD:
			case EVERGREEN:
				return R.color.filter_color_blue_dark_dark;
			case FAIRYTALE:
				return R.color.filter_color_blue;
			case ROMANCE:
			case SAKURA:
			case WARM:
				return R.color.filter_color_pink;
			case AMARO:
			case BRANNAN:
			case BROOKLYN:
			case EARLYBIRD:
			case FREUD:
			case HEFE:
			case HUDSON:
			case INKWELL:
			case KEVIN:
			case LOMO:
			case N1977:
			case NASHVILLE:
			case PIXAR:
			case RISE:
			case SIERRA:
			case SUTRO:
			case TOASTER2:
			case VALENCIA:
			case WALDEN:
			case XPROII:
				return R.color.filter_color_brown_dark;
			case ANTIQUE:
			case NOSTALGIA:
				return R.color.filter_color_green_dark;
			case SKINWHITEN:
			case HEALTHY:
				return R.color.filter_color_red;
			case SWEETS:
				return R.color.filter_color_red_dark;
			case CALM:
			case LATTE:
			case TENDER:
				return R.color.filter_color_brown;
			default:
				return R.color.filter_color_grey_light;
		}
	}
	
	public static int FilterType2Thumb(int type){
		MagicFilterType filterType = MagicFilterType.values()[type];
		switch (filterType) {
		case NONE:
			return R.drawable.filter_thumb_original;
		case WHITECAT:
			return R.drawable.filter_thumb_whitecat;
		case BLACKCAT:
			return R.drawable.filter_thumb_blackcat;
		case ROMANCE:
			return R.drawable.filter_thumb_romance;
		case SAKURA:
			return R.drawable.filter_thumb_sakura;
		case AMARO:
			return R.drawable.filter_thumb_amoro;
		case BRANNAN:
			return R.drawable.filter_thumb_brannan;
		case BROOKLYN:
			return R.drawable.filter_thumb_brooklyn;
		case EARLYBIRD:
			return R.drawable.filter_thumb_earlybird;
		case FREUD:
			return R.drawable.filter_thumb_freud;
		case HEFE:
			return R.drawable.filter_thumb_hefe;
		case HUDSON:
			return R.drawable.filter_thumb_hudson;
		case INKWELL:
			return R.drawable.filter_thumb_inkwell;
		case KEVIN:
			return R.drawable.filter_thumb_kevin;
		case LOMO:
			return R.drawable.filter_thumb_lomo;
		case N1977:
			return R.drawable.filter_thumb_1977;
		case NASHVILLE:
			return R.drawable.filter_thumb_nashville;
		case PIXAR:
			return R.drawable.filter_thumb_piaxr;
		case RISE:
			return R.drawable.filter_thumb_rise;
		case SIERRA:
			return R.drawable.filter_thumb_sierra;
		case SUTRO:
			return R.drawable.filter_thumb_sutro;
		case TOASTER2:
			return R.drawable.filter_thumb_toastero;
		case VALENCIA:
			return R.drawable.filter_thumb_valencia;
		case WALDEN:
			return R.drawable.filter_thumb_walden;
		case XPROII:
			return R.drawable.filter_thumb_xpro;
		case ANTIQUE:
			return R.drawable.filter_thumb_antique;
		case SKINWHITEN:
			return R.drawable.filter_thumb_beauty;
		case CALM:
			return R.drawable.filter_thumb_calm;
		case COOL:
			return R.drawable.filter_thumb_cool;
		case EMERALD:
			return R.drawable.filter_thumb_emerald;
		case EVERGREEN:
			return R.drawable.filter_thumb_evergreen;
		case FAIRYTALE:
			return R.drawable.filter_thumb_fairytale;
		case HEALTHY:
			return R.drawable.filter_thumb_healthy;
		case NOSTALGIA:
			return R.drawable.filter_thumb_nostalgia;
		case TENDER:
			return R.drawable.filter_thumb_tender;
		case SWEETS:
			return R.drawable.filter_thumb_sweets;
		case LATTE:
			return R.drawable.filter_thumb_latte;
		case WARM:
			return R.drawable.filter_thumb_warm;
		case SUNRISE:
			return R.drawable.filter_thumb_sunrise;
		case SUNSET:
			return R.drawable.filter_thumb_sunset;
		case CRAYON:
			return R.drawable.filter_thumb_crayon;
		case SKETCH:
			return R.drawable.filter_thumb_sketch;
		default:
			return R.drawable.filter_thumb_original;
		}
	}
	
	public static int FilterType2Name(int type){
		MagicFilterType filterType = MagicFilterType.values()[type];
		switch (filterType) {
		case NONE:
			return R.string.filter_none;
		case SOULOUT:
			return R.string.filter_soul_out;
		case WHITECAT:
			return R.string.filter_whitecat;
		case BLACKCAT:
			return R.string.filter_blackcat;
		case ROMANCE:
			return R.string.filter_romance;
		case SAKURA:
			return R.string.filter_sakura;
		case AMARO:
			return R.string.filter_amaro;
		case BRANNAN:
			return R.string.filter_brannan;
		case BROOKLYN:
			return R.string.filter_brooklyn;
		case EARLYBIRD:
			return R.string.filter_Earlybird;
		case FREUD:
			return R.string.filter_freud;
		case HEFE:
			return R.string.filter_hefe;
		case HUDSON:
			return R.string.filter_hudson;
		case INKWELL:
			return R.string.filter_inkwell;
		case KEVIN:
			return R.string.filter_kevin;
		case LOMO:
			return R.string.filter_lomo;
		case N1977:
			return R.string.filter_n1977;
		case NASHVILLE:
			return R.string.filter_nashville;
		case PIXAR:
			return R.string.filter_pixar;
		case RISE:
			return R.string.filter_rise;
		case SIERRA:
			return R.string.filter_sierra;
		case SUTRO:
			return R.string.filter_sutro;
		case TOASTER2:
			return R.string.filter_toastero;
		case VALENCIA:
			return R.string.filter_valencia;
		case WALDEN:
			return R.string.filter_walden;
		case XPROII:
			return R.string.filter_xproii;
		case ANTIQUE:
			return R.string.filter_antique;
		case CALM:
			return R.string.filter_calm;
		case COOL:
			return R.string.filter_cool;
		case EMERALD:
			return R.string.filter_emerald;
		case EVERGREEN:
			return R.string.filter_evergreen;
		case FAIRYTALE:
			return R.string.filter_fairytale;
		case HEALTHY:
			return R.string.filter_healthy;
		case NOSTALGIA:
			return R.string.filter_nostalgia;
		case TENDER:
			return R.string.filter_tender;
		case SWEETS:
			return R.string.filter_sweets;
		case LATTE:
			return R.string.filter_latte;
		case WARM:
			return R.string.filter_warm;
		case SUNRISE:
			return R.string.filter_sunrise;
		case SUNSET:
			return R.string.filter_sunset;
		case SKINWHITEN:
			return R.string.filter_skinwhiten;
		case CRAYON:
			return R.string.filter_crayon;
		case SKETCH:
			return R.string.filter_sketch;
		default:
			return R.string.filter_none;
		}
	}
}
