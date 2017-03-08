/*
 * CisloSlovom.h
 *
 *  Created on: 4.6.2013
 *      Author: voicegroup
 */

#ifndef CISLOSLOVOM_H_
#define CISLOSLOVOM_H_

#include <string>
#include <cassert>
using namespace std;
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author jules
 */

enum GrammarNames {
	 NEDEF = 0,

	 ROD_MUZ = 10,
	ROD_MUZN = 11,
	 ROD_ZENA = 12,
	 ROD_STR = 13,

	 PAD_NOM = 1,
	 PAD_GEN = 2,
	 PAD_DAT = 3,
	PAD_AK = 4,
	PAD_VOK = 5,
	PAD_LOK = 6,
	PAD_INS = 7,


	DRUH_POD = 21,
	 DRUH_PRID = 22,
	 DRUH_ZAM = 23,
	DRUH_CIS = 24,
	 DRUH_SLOV = 25,
	DRUH_PRIS = 26,
	DRUH_URC = 29,
	DRUH_CIT = 30,
	DRUH_SPOJ = 31,
	DRUH_PRED = 32,

	 CISLO_JEDN = 40,
	 CISLO_MNOZ = 41,
};

class CisloSlovom {

    static string pripona(int hodnota,int rod,int pad){
   	string pripona = "";
   	//bool dlzen = false;
   	//if (hodnota == 1 || hodnota == 2 ||hodnota == 3 || hodnota == 4){
   	//	dlzen = true;
   	//}
   	if(pad ==  PAD_NOM || pad ==  PAD_VOK){
	    	if(rod ==  ROD_MUZ){
	        	pripona = "";
	        	if(hodnota == 1 )
	        		pripona = "en";
	        	if(hodnota == 3 || hodnota == 2)
	        		pripona = "aja";
	        	if(hodnota == 4 )
	        		pripona = "ia";
	        }
	    	else {
	        	pripona = "";
	        	if(hodnota == 1){
	        		pripona = "na";
	        		if(rod ==  ROD_STR)
		        		pripona = "no";
	        		if(rod ==  ROD_STR && hodnota > 10)
		        		pripona = "en";
		        	if(rod ==  ROD_MUZN)
		        		pripona = "en";
	        	}


	        	if(hodnota == 2 )
	        		pripona = "e";
	        	if(hodnota == 2 && rod ==  ROD_MUZN)
	        		pripona = "a";
	        	if(hodnota == 3 )
	        		pripona = "i";
	        	if(hodnota == 4 )
	        		pripona = "yri";
	        }
	    }
	    else if(pad ==  PAD_GEN){
	    	pripona = "ich";
	    	if(hodnota == 1 )
       		pripona = "ných";
	    	if(hodnota == 1 && (rod ==  ROD_ZENA ))
       		pripona = "nej";
	       	if(hodnota == 2 || hodnota == 3|| hodnota == 4)
	       		pripona = "och";
	       	if(hodnota == 12 || hodnota == 13|| hodnota == 14)
	       		pripona = "ich";
	    }
	    else if(pad ==  PAD_DAT){
	    		pripona = "im";
	    		if(hodnota == 1 )
	        		pripona = "nému";
		    	if(hodnota == 1 && (rod ==  ROD_ZENA ))
	        		pripona = "nej";
		       	if(hodnota == 2 || hodnota == 3|| hodnota == 4)
		       		pripona = "om";
	    }
	    else if(pad ==  PAD_AK){
	    	if(rod ==  ROD_MUZ){
	        	pripona = "ich";
	        	if(hodnota == 1 )
	        		pripona = "ného";
	        	if(hodnota == 2 || hodnota == 3 || hodnota == 4)
	        		pripona = "och";

	        }
	    	else {
	    		pripona = "";
	    		if(hodnota == 1 && (rod ==  ROD_ZENA ))
	        		pripona = "nu";
	    		if(hodnota == 1 && (rod ==  ROD_STR ))
	        		pripona = "nu";
	    		if(hodnota == 1 && (rod ==  ROD_MUZN ))
	        		pripona = "en";
	    		if(hodnota == 2 && (rod ==  ROD_ZENA  ||rod ==  ROD_STR ))
	        		pripona = "e";
	    		if(hodnota == 2 && (rod ==  ROD_MUZN ))
	        		pripona = "a";
	        	if(hodnota == 3 || hodnota == 4)
	        		pripona = "i";
	        }
	    }
	    else if(pad ==  PAD_LOK){
	    	pripona = "ich";
	    	if(hodnota == 1)
       		pripona = "nom";

	    	if(hodnota == 1 && rod ==  ROD_ZENA)
       		pripona = "nej";
	    	if(hodnota == 2|| hodnota == 3||hodnota == 4)
       		pripona = "och";


	    }
	    else if(pad ==  PAD_INS){
	        pripona = "imi";
	        if(hodnota == 1)
       		pripona = "ným";
	    	if(hodnota == 1 && rod ==  ROD_ZENA)
       		pripona = "nou";
	    	if(hodnota == 3 || hodnota == 2 )
	      		pripona = "omi";
	        if(hodnota == 4)
	      		pripona = "mi";
	    }
	    return pripona;
   }
	/**
	 * Zakladna cislovka
	 * @param cislo
	 * @return
	 */
	 static string cisloSlovom(int cislo){
		if(cislo == 0){
			return "nula";
		}
		if(cislo >= 1000){
			return cisloSlovomMilion(cislo);
		}
		return cisloSlovomMilion(cislo);
	}
	 /**
	 * Radova cislovka, nominativ
	 * do 99
	 * @param hodnota
	 * @return
	 */
	 static string cisloSlovomR(int hodnota,int rod){
		if(hodnota >= 1000){
			return cisloSlovomMilion(hodnota);
		}
		return cisloSlovom100999R(hodnota, rod,PAD_NOM, CISLO_JEDN);
	}

	 static string cisloSlovom19N(int hodnota){
		if(hodnota == 1){
			return "jeden";
		}
		return cisloSlovom19N(hodnota,  ROD_STR);
	}
     static string cisloSlovom19N(int hodnota,int rod) {
        string result = "";
        if (hodnota < 10) {
            switch (hodnota) {
//            	case 0:
//            		result = "nula";
//            		break;
                case 1:
                    result = "jeden";
//                    if(rod ==  ROD_STR){
//                    	result = "jedno";
//                    }
//                    else
                    	if(rod ==  ROD_ZENA){
                    	result = "jedna";
                    }
                    break;
                case 2:
                    result = "dva";
                    if(rod ==  ROD_MUZ){
                    	result = "dvaja";
                    }
                    else if(rod ==  ROD_ZENA){
                    	result = "dve";
                    }
                    break;
                case 3:
                    result = "tri";
                    if(rod ==  ROD_MUZ){
                    	result = "traja";
                    }
                    break;
                case 4:
                    result = "štyri";
                    if(rod ==  ROD_MUZ){
                    	result = "štyria";
                    }
                    break;
                case 5:
                    result = "päť";
                    if(rod ==  ROD_MUZ){
                    	result = "piati";
                    }
                    break;
                case 6:
                    result = "šesť";
                    if(rod ==  ROD_MUZ){
                    	result = "šiesti";
                    }
                    break;
                case 7:
                    result = "sedem";
                    if(rod ==  ROD_MUZ){
                    	result = "siedmi";
                    }
                    break;
                case 8:
                    result = "osem";
                    if(rod ==  ROD_MUZ){
                    	result = "ôsmi";
                    }
                    break;
                case 9:
                    result = "deväť";
                    if(rod ==  ROD_MUZ){
                    	result = "deviati";
                    }
                    break;
            }
        }
        return result;
    }
     static string cisloSlovom19(int hodnota,int rod, int pad) {
        string result = "";
        if (hodnota < 10) {
        	result = cislo19(hodnota) + pripona(hodnota, rod, pad);
        	if(pad ==  PAD_NOM || pad ==  PAD_VOK){
            	result = cisloSlovom19N(hodnota,rod);
            }
        	if((pad ==  PAD_AK || pad ==  PAD_GEN) && rod !=  ROD_MUZ){
            	result = cisloSlovom19N(hodnota,rod);
            }
        }

        return result;
    }

     static string cislo19(int hodnota) {
        string result = "";
        if (hodnota < 10) {

            switch (hodnota) {
//            	case 0:
//            		result = "nula";
//            		break;
                case 1:
                    result = "jed";
                    break;
                case 2:
                    result = "dv";
                    break;
                case 3:
                    result = "tr";
                    break;
                case 4:
                    result = "štyr";
                    break;
                case 5:
                    result = "piat";
                    break;
                case 6:
                    result = "šiest";
                    break;
                case 7:
                    result = "siedm";
                    break;
                case 8:
                    result = "ôsm";
                    break;
                case 9:
                    result = "deviat";
                    break;
            }
        }
        return result;
    }
     static string cisloR(int cislo) {
        string result = "";
        if (cislo < 10) {
        	switch (cislo) {
            case 0:
                result = "nult";
                break;
                case 1:
                    result = "prv";
                    break;
                case 2:
                    result = "druh";
                    break;
                case 3:
                    result = "tret";
                    break;
                case 4:
                    result = "štvrt";
                    break;
                case 5:
                    result = "piat";
                    break;
                case 6:
                    result = "šiest";
                    break;
                case 7:
                    result = "siedm";
                    break;
                case 8:
                    result = "ôsm";
                    break;
                case 9:
                    result = "deviat";
                    break;
            }
        }
        return result;
    }


     static string cisloSlovom19R(int hodnota,int rod,int pad,int cislo) {
        string result = "";
        string pripona = "";
        pripona = priponaR(hodnota, rod, pad, cislo);
        result = cisloR(hodnota)+pripona;
        return result;
}

     static string cisloSlovom1019N(int hodnota) {
        if (hodnota < 10) {
            return cisloSlovom19N(hodnota, ROD_STR);
        }
        hodnota = hodnota - 10;
        string result = "";
        switch (hodnota) {
            case 0:
                result = "desať";
                break;
            case 1:
                result = cisloSlovom19N(hodnota) + "ásť";
                break;
            case 2:
                result = cisloSlovom19N(hodnota) + "násť";
                break;
            case 3:
                result = cisloSlovom19N(hodnota) + "násť";
                break;
            case 4:
                result = "štrnásť";
                break;
            case 5:
                result = "pätnásť";
                break;
            case 6:
                result = "šestnásť";
                break;
            case 7:
                result = cisloSlovom19N(hodnota, ROD_STR) + "násť";
                break;
            case 8:
                result = cisloSlovom19N(hodnota, ROD_STR) + "násť";
                break;
            case 9:
                result = "devätnásť";
        }
        return result;
    }
     static string cisloSlovom1019(int hodnota,int rod,int pad) {
    	if(pad ==  PAD_NOM ||pad ==  PAD_VOK ){
        	return cisloSlovom1019N(hodnota);
        }
    	if(pad ==  PAD_AK && rod !=  ROD_MUZ){
        	return  cisloSlovom1019N(hodnota);
        }
    	if (hodnota < 10) {
            return cisloSlovom19(hodnota,rod,pad);
        }

        string result = "";

        switch (hodnota) {
            case 10:
                result = "desiat" ;
                break;
            case 11:
                result = "jedenást";
                break;
            case 12:
                result = "dvanást";
                break;
            case 13:
                result = "trinást";
                break;
            case 14:
                result = "štrnást";
                break;
            case 15:
                result = "pätnást";
                break;
            case 16:
                result = "šestnást";
                break;
            case 17:
                result = "sedemnást";
                break;
            case 18:
                result = "osemnást";
                break;
            case 19:
                result = "devätnást";
        }
        result += pripona(hodnota, rod, pad);
        return result;
    }





     static string cisloSlovom1019R(int hodnota,int rod,int pad,int cislo) {
        if (hodnota < 10) {
            return cisloSlovom19R(hodnota,rod,pad,cislo);
        }
        string pripona = "";
        string result ="";
        	pripona = priponaR(hodnota,rod,pad,cislo);
        	result =  cisloSlovom1019R(hodnota,rod,pad,cislo,pripona);
        return result;
    }
     static string cisloSlovom1019R(int hodnota,int rod,int pad,int cislo,string pripona) {
        if (hodnota < 10) {
            return cisloSlovom19R(hodnota,rod,pad,cislo);
        }
        hodnota = hodnota - 10;
        string result = "";
        pripona = pripona + " ";
        switch (hodnota) {
            case 0:
                result = "desiat" + pripona;
                break;
            case 1:
                result = "jedenást" + pripona;
                break;
            case 2:
                result = "dvanást" + pripona;
                break;
            case 3:
                result = "trinást" + pripona;
                break;
            case 4:
                result = "štrnást" + pripona;
                break;
            case 5:
                result = "pätnást" + pripona;
                break;
            case 6:
                result = "šestnást" + pripona;
                break;
            case 7:
                result = "sedemnást" + pripona;
                break;
            case 8:
                result = "osemnást" + pripona;
                break;
            case 9:
                result = "devätnást" + pripona;
        }
        return result;
    }

     static string cisloSlovom2099N(int cislo) {
        if (cislo < 20) {
            return cisloSlovom1019N(cislo);
        }
        string result = "";
        // Celociselne delenie
        int desiatky = cislo / 10;
        if (desiatky == 0) {
            return cisloSlovom19N(cislo);
        }
        int jednotky = cislo % 10;

        if (desiatky == 1) {
            result = cisloSlovom1019N(cislo);
        }
        if (desiatky == 2 ||desiatky == 3 ||  desiatky == 4) {
            result = cisloSlovom19N(desiatky) + "dsať" +" " + cisloSlovom19N(jednotky);
        }
        else if (desiatky >= 5 && desiatky <= 8) {
            result = cisloSlovom19N(desiatky) + "desiat "  + cisloSlovom19N(jednotky);
        }
        else if (desiatky == 9) {
            result = "deväťdesiat "  + cisloSlovom1019N(jednotky);
        }
        return result;
    }
     static string cisloSlovom2099(int hodnota,int rod, int pad) {
    	if (hodnota < 10 && (pad ==  PAD_NOM || pad ==  PAD_VOK)) {
            return cisloSlovom19N(hodnota,rod);
        }
    	if (hodnota < 20) {
            return cisloSlovom1019(hodnota,rod,pad);
        }
        if(pad ==  PAD_NOM || pad ==  PAD_VOK){
        	return  cisloSlovom2099N(hodnota);
        }
        if(pad ==  PAD_AK && rod !=  ROD_MUZ){
        	return  cisloSlovom2099N(hodnota);
        }
        string result = "";

        // Celociselne delenie
        int desiatky = hodnota / 10;
        int jednotky = hodnota % 10;

        if (desiatky == 2 ||desiatky == 3 ||desiatky == 4) {
            result = cisloSlovom19N(desiatky) + "dsiat";
        }
        else if (desiatky >= 5 && desiatky <= 8) {
            result = cisloSlovom19N(desiatky) + "desiat";
        }
        else if (desiatky == 9) {
            result = "deväťdesiat";
        }
        string pr = pripona(hodnota, rod, pad);
        result += pr +" "  ;
        if(jednotky != 0){
        	result += cisloSlovom19(jednotky,rod,pad);
        }
        return result;
    }



     static string cisloSlovom2099R(int hodnota,int rod,int pad,int cislo,string pripona) {
        if (hodnota < 20) {
            return cisloSlovom1019R(hodnota,rod,pad,cislo);
        }
        string result = "";
        pripona = pripona + " ";
        // Celociselne delenie
        int desiatky = hodnota / 10;
        int jednotky = hodnota % 10;

        if (desiatky == 1) {
            result = cisloSlovom1019R(hodnota,rod,pad,cislo);
        }
        else if (desiatky == 2 || desiatky == 3 || desiatky == 4) {
            result = cisloSlovom19N(desiatky) + "dsiat" + pripona;
        }
        else if (desiatky >= 5 && desiatky <= 8) {
            result = cisloSlovom19N(desiatky) + "desiat" + pripona;
        }
        else if (desiatky == 9) {
            result = "deväťdesiat" + pripona;
        }

        if(jednotky != 0){
        	result += cisloSlovom1019R(jednotky,rod,pad,cislo);
        }
        return result;
    }

     static string cisloSlovom2099R(int hodnota,int rod,int pad,int cislo) {
	    if (hodnota < 20) {
	        return cisloSlovom1019R(hodnota,rod,pad,cislo);
	    }
	    string pripona = priponaR(hodnota,rod,pad,cislo);
	    string result =  cisloSlovom2099R(hodnota,rod,pad,cislo,pripona);
	    return result;
	}

     static string priponaR(int hodnota,int rod,int pad,int cislo){
    	string pripona = "";
    	bool dlzen = false;
    	bool mnoznecislo = false;
    	if (hodnota == 0 ||hodnota == 1 || hodnota == 2 ||hodnota == 3 || hodnota == 4){
    		dlzen = true;
    	}

    	if(cislo ==  CISLO_MNOZ){
    		mnoznecislo = true;
    	}
	    if(pad ==  PAD_NOM || pad ==  PAD_VOK){
	    	if(rod ==  ROD_MUZ){
	        	pripona = "y";
	        	if(dlzen)
	        		pripona = "ý";
	        	if(hodnota == 3)
	        		pripona = "í";
	        	if(mnoznecislo){
	        		pripona  = "i";
	        		if(dlzen)
		        		pripona  = "í";

	        	}
	        }
	    	else if(rod ==  ROD_MUZN){
	        	pripona = "y";
	        	if(dlzen)
	        		pripona = "ý";
	        	if(hodnota == 3)
	        		pripona = "í";
	        	if(mnoznecislo){
	        		pripona  = "e";
	        		if(dlzen)
		        		pripona  = "é";
	        		if(hodnota == 3)
		        		pripona = "ie";

	        	}
	        }
	    	else if(rod ==  ROD_ZENA){
	        	pripona = "a";
	        	if(dlzen)
	        		pripona = "á";
	        	if(hodnota == 3)
	        		pripona = "ia";
	        	if(mnoznecislo){
	        		pripona  = "e";
	        		if(dlzen)
		        		pripona  = "é";
	        		if(hodnota == 3){
	        			pripona  = "ie";
	        		}
	        	}
	        }
	    	else if(rod ==  ROD_STR){
	        	pripona = "e";
	        	if(dlzen)
	        		pripona = "é";
	        	if(hodnota == 3){
	        		pripona  = "ie";
	        	}
	        	if(mnoznecislo){
	        		pripona  = "e";
	        		if(dlzen)
		        		pripona  = "é";
	        		if(hodnota == 3){
	        			pripona  = "ie";
	        		}
	        	}
	        }
	    }
	    else if(pad ==  PAD_GEN){
	    	if(rod ==  ROD_MUZ || rod ==  ROD_MUZN || rod ==  ROD_STR){
	        	pripona = "eho";
	        	if(dlzen)
	        		pripona = "ého";
	        	if(hodnota == 3)
	        		pripona = "ieho";
	        }
	    	else if(rod ==  ROD_ZENA){
	        	pripona = "ej";
	        }
	    	if(mnoznecislo){
        		pripona  = "ych";
        		if(dlzen){
        			pripona  = "ých";
        		}
        		if(hodnota == 3)
	        		pripona = "ích";
	    	}
	    }
	    else if(pad ==  PAD_DAT){
	    	if(rod ==  ROD_MUZ || rod ==  ROD_MUZN || rod ==  ROD_STR){
	        	pripona = "emu";
	        	if(dlzen)
	        		pripona = "ému";
	        	if(hodnota == 3)
	        		pripona = "iemu";
	        }
	    	else if(rod ==  ROD_ZENA){
	        	pripona = "ej";
	        }
	    	if(mnoznecislo){
        		pripona  = "ym";
        		if(dlzen)
	        		pripona = "ým";
        		if(hodnota == 3)
	        		pripona = "ím";
	    	}
	    }
	    else if(pad ==  PAD_AK){
	    	if(rod ==  ROD_MUZ|| rod ==  ROD_MUZN ||rod ==  ROD_STR){
	        	pripona = "eho";
	        	if(dlzen)
	        		pripona = "ého";
	        	if(hodnota == 3)
	        		pripona = "ieho";
	        }
	    	else if(rod ==  ROD_ZENA){
	        	pripona = "u";
	        	if(dlzen)
	        		pripona = "ú";
	        	if(hodnota == 3)
	        		pripona = "iu";
	        }
	    	if(mnoznecislo){
        		pripona  = "ych";
        		if(dlzen)
	        		pripona = "ých";
        		if(hodnota == 3)
	        		pripona = "ích";
	    	}
	    }
	    else if(pad ==  PAD_LOK){
	    	if(rod ==  ROD_MUZ||rod ==  ROD_MUZN || rod ==  ROD_STR){
	        	pripona = "om";
	        }
	    	else if(rod ==  ROD_ZENA){
	        	pripona = "ej";
	        }
	    	if(mnoznecislo){
        		pripona  = "ych";
        		if(dlzen)
	        		pripona = "ých";
        		if(hodnota == 3)
	        		pripona = "ích";
	    	}
	    }
	    else if(pad ==  PAD_INS){
	    	if(rod ==  ROD_MUZ|| rod ==  ROD_MUZN ||rod ==  ROD_STR){
	        	pripona = "ym";
	        	if(dlzen)
	        		pripona = "ým";
	        	if(hodnota == 3)
	        		pripona = "ím";
	        }
	    	else if(rod ==  ROD_ZENA){
	        	pripona = "ou";
	        }
	    	if(mnoznecislo){
        		pripona  = "ymi";
        		if(dlzen)
	        		pripona = "ými";
        		if(hodnota == 3)
	        		pripona = "ími";
	    	}
	    }
	    return pripona;
    }

	 static string mesiacSlovom(int mesiac){
    	string result = "";
    	switch (mesiac) {
		case 1:
			result = "januára";
			break;
		case 2:
			result = "februára";
			break;
		case 3:
			result = "marca";
			break;
		case 4:
			result = "apríla";
			break;
		case 5:
			result = "mája";
			break;
		case 6:
			result = "júna";
			break;
		case 7:
			result = "júla";
			break;
		case 8:
			result = "augusta";
			break;
		case 9:
			result = "septembra";
			break;
		case 10:
			result = "októbra";
			break;
		case 11:
			result = "novembra";
			break;
		case 12:
			result = "decembra";
			break;
		}
    	return result;
    }

     static string cisloSlovom100999N(int cislo) {
        if (cislo < 100) {
            return cisloSlovom2099N(cislo);
        }
        string result = "";
        // Celociselne delenie
        int stovky = cislo / 100;
        int desiatky = cislo % 100;
        if (stovky == 1) {
            result = "sto " +cisloSlovom2099N(desiatky);
        }
        if (stovky == 2) {
            result = "dvesto " +cisloSlovom2099N(desiatky);
        }
        if (stovky >= 3) {
            result = cisloSlovom19N(stovky) + "sto " + cisloSlovom2099N(desiatky);
        }
        return result;
    }
     static string cisloSlovom100999(int hodnota,int rod,int pad) {
        if (hodnota < 100) {
            return cisloSlovom2099(hodnota,rod,pad);
        }
        string result = "";
        // Celociselne delenie
        int stovky = hodnota / 100;
        int desiatky = hodnota % 100;
        if (stovky == 1) {
            result = "sto";
            if(desiatky != 0){
            	result += " "+cisloSlovom2099(desiatky,rod,pad);;
            }
        }
        else if (stovky == 2) {
            result = "dvesto " +cisloSlovom2099(desiatky,rod,pad);
        }
        else if (stovky >= 3) {
            result = cisloSlovom19N(stovky) + "sto " + cisloSlovom2099(desiatky,rod,pad);
        }
        if(desiatky == 0){
        	result = stovkyR(stovky, rod, pad, CISLO_JEDN);
        }
        return result;
    }

     static string stovkyR(int stovky,int rod,int pad,int cislo){
    	string result = "";
    	string pripona = priponaR(1,rod,pad,cislo);
    	switch (stovky) {
		case 1:
			result = "st" + pripona;
			break;
		case 2:
			result = "dvest" + pripona;
			break;
		case 3:
			result = "trojst" + pripona;
			break;
		case 4:
			result = "štvorst" + pripona;
			break;
		default:
			result = cisloSlovom19N(stovky) + "st" + pripona;
			break;
		}
    	return result;
    }

     static string tisicky(int hodnota,int rod,int pad,int cislo){
    	string result = "";

    	hodnota = hodnota / 1000;
    	//System.out.println(hodnota);
    	switch (hodnota) {
		case 1:
			result = "tisíc";
			break;
		case 2:
			result = "dvetisíc";
			break;
		default:
			result = cisloSlovom19N(hodnota) + " tisíc";
			break;
		}
    	//result += pripona;
    	return result;
    }




     static string cisloSlovom100999R(int hodnota,int rod,int pad,int cislo) {
        if (hodnota < 100) {
            return cisloSlovom2099R(hodnota,rod,pad,cislo);
        }
        string result = "";
        // Celociselne delenie
        int stovky = hodnota / 100;
        int desiatky = hodnota % 100;
        if (stovky == 1) {
            result = "sto "+ cisloSlovom2099R(desiatky,rod,pad,cislo);
        }
        else if (stovky == 2) {
            result = "dvesto "+ cisloSlovom2099R(desiatky,rod,pad,cislo);
        }
        else if (stovky >= 3) {
            result = cisloSlovom19N(stovky) + "sto " + cisloSlovom2099R(desiatky,rod,pad,cislo);
        }
        if(desiatky == 0){
        	result = stovkyR(stovky, rod, pad,cislo);
        }
        return result;
    }



     static string cisloSlovom1000999999N(int cislo) {
        if (cislo < 1000) {
            return cisloSlovom100999N(cislo);
        }
        int doTisic = cislo % 1000;
        int nadTisic = cislo / 1000;
        string result = "";
        if (nadTisic == 1) {
            result = "tisíc " + cisloSlovom100999N(doTisic);
        }
        if (nadTisic == 2) {
            result = "dvetisíc " + cisloSlovom100999N(doTisic);
        }
        if (nadTisic > 2) {
            result = cisloSlovom100999N(nadTisic)+" tisíc " + cisloSlovom100999N(doTisic);
        }
        return result;
    }

     static string cisloSlovom1000999999(int hodnota,int rod,int pad) {
        if (hodnota < 1000) {
            return cisloSlovom100999(hodnota,rod,pad);
        }
        return cisloSlovom1000999999N(hodnota);
//        int doTisic = hodnota % 1000;
//        int nadTisic = hodnota / 1000;
//        string result = "";
//        string pripona = pripona(doTisic, rod, pad);
//        result = tisícky(hodnota, rod, pad,  CISLO_JEDN) ;
//        if(doTisic > 4){
//        	result += pripona;
//        }
//        if(doTisic > 0){
//        	result += " " + cisloSlovom100999(doTisic, rod, pad);
//        }
//        if (nadTisic == 1) {
//            result = "tisíc" +" " + cisloSlovom100999(doTisic,rod,pad);
//        }
//        if (nadTisic == 2) {
//            result = "dvetisíc"+" " + cisloSlovom100999(doTisic,rod,pad);
//        }
//        if (nadTisic > 2) {
//            result = cisloSlovom100999N(nadTisic)+" " + "tisíc" +" " + cisloSlovom100999(doTisic,rod,pad);
//        }
        //return result;
    }

     static string cisloSlovomMilion(int cislo) {
        if (cislo < 1000000) {
            return cisloSlovom1000999999N(cislo);
        }
        string result = "";
        int nadMilion = cislo / 1000000;
        int podMilion = cislo % 1000000;
        if (nadMilion == 1) {
            result = "milión "+ cisloSlovom1000999999N(podMilion);
        }
        if (nadMilion >= 2 && nadMilion <= 4) {
            result = cisloSlovom1000999999N(nadMilion) +" milióny " + cisloSlovom1000999999N(podMilion);
        }
        if (nadMilion >= 5) {
            result = cisloSlovom1000999999N(nadMilion) +" miliónov " + cisloSlovom1000999999N(podMilion);
        }
        return result;
    }
public:
     static string cisloSlovomR(int hodnota,int rod=ROD_MUZN,int pad=PAD_NOM,int cislo=CISLO_JEDN){
    	 assert(pad > 0);

    	 if(hodnota >= 1000){
     			return cisloSlovomMilion(hodnota);
     		}
     		return cisloSlovom100999R(hodnota, rod,pad,cislo);
     	}

     static string cisloSlovom(int hodnota,int rod=ROD_MUZN,int pad=PAD_NOM){
    	 assert(pad > 0);
     		if(hodnota == 0){
     			return "nula";
     		}
     		if(hodnota >= 1000){
     			return cisloSlovomMilion(hodnota);
     		}
     		return cisloSlovom1000999999(hodnota, rod, pad);
     	}



};



#endif /* CISLOSLOVOM_H_ */
