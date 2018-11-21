//package com.cangwang.magic.helper;
//
//public class MagicFilterFactory{
//
//	private static MagicFilterType filterType = MagicFilterType.NONE;
//
//	public static GPUImageFilter initFilters(MagicFilterType type){
//		filterType = type;
//		switch (type) {
//		case WHITECAT:
//			return new MagicWhiteCatFilter();
//		case BLACKCAT:
//			return new MagicBlackCatFilter();
//		case SKINWHITEN:
//			return new MagicSkinWhitenFilter();
//		case ROMANCE:
//			return new MagicRomanceFilter();
//		case SAKURA:
//			return new MagicSakuraFilter();
//		case AMARO:
//			return new MagicAmaroFilter();
//		case WALDEN:
//			return new MagicWaldenFilter();
//		case ANTIQUE:
//			return new MagicAntiqueFilter();
//		case CALM:
//			return new MagicCalmFilter();
//		case BRANNAN:
//			return new MagicBrannanFilter();
//		case BROOKLYN:
//			return new MagicBrooklynFilter();
//		case EARLYBIRD:
//			return new MagicEarlyBirdFilter();
//		case FREUD:
//			return new MagicFreudFilter();
//		case HEFE:
//			return new MagicHefeFilter();
//		case HUDSON:
//			return new MagicHudsonFilter();
//		case INKWELL:
//			return new MagicInkwellFilter();
//		case KEVIN:
//			return new MagicKevinFilter();
//		case LOMO:
//			return new MagicLomoFilter();
//		case N1977:
//			return new MagicN1977Filter();
//		case NASHVILLE:
//			return new MagicNashvilleFilter();
//		case PIXAR:
//			return new MagicPixarFilter();
//		case RISE:
//			return new MagicRiseFilter();
//		case SIERRA:
//			return new MagicSierraFilter();
//		case SUTRO:
//			return new MagicSutroFilter();
//		case TOASTER2:
//			return new MagicToasterFilter();
//		case VALENCIA:
//			return new MagicValenciaFilter();
//		case XPROII:
//			return new MagicXproIIFilter();
//		case EVERGREEN:
//			return new MagicEvergreenFilter();
//		case HEALTHY:
//			return new MagicHealthyFilter();
//		case COOL:
//			return new MagicCoolFilter();
//		case EMERALD:
//			return new MagicEmeraldFilter();
//		case LATTE:
//			return new MagicLatteFilter();
//		case WARM:
//			return new MagicWarmFilter();
//		case TENDER:
//			return new MagicTenderFilter();
//		case SWEETS:
//			return new MagicSweetsFilter();
//		case NOSTALGIA:
//			return new MagicNostalgiaFilter();
//		case FAIRYTALE:
//			return new MagicFairytaleFilter();
//		case SUNRISE:
//			return new MagicSunriseFilter();
//		case SUNSET:
//			return new MagicSunsetFilter();
//		case CRAYON:
//			return new MagicCrayonFilter();
//		case SKETCH:
//			return new MagicSketchFilter();
//		//image adjust
//		case BRIGHTNESS:
//			return new GPUImageBrightnessFilter();
//		case CONTRAST:
//			return new GPUImageContrastFilter();
//		case EXPOSURE:
//			return new GPUImageExposureFilter();
//		case HUE:
//			return new GPUImageHueFilter();
//		case SATURATION:
//			return new GPUImageSaturationFilter();
//		case SHARPEN:
//			return new GPUImageSharpenFilter();
//		case IMAGE_ADJUST:
//			return new MagicImageAdjustFilter();
//		default:
//			return null;
//		}
//	}
//
//	public MagicFilterType getCurrentFilterType(){
//		return filterType;
//	}
//}
