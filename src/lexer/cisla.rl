%%{
machine Cisla;

cisloslovom =
('jeden' |
'dva' |
'tri' |
'štyri' |
'päť' |
'šesť' |
'sedem' |
'osem' |
'deväť'|
'desať' |
'jedenásť' |
'dvanásť' |
'trinásť' |
'štrnásť' |
'pätnásť' |
'šestnásť' |
'sedemnásť' |
'osemnásť' |
'devätnásť'|
'dvadsať' |
'tridsať' |
'štyridsať' |
'päťdesiat' |
'šesťdesiat' |
'sedemdesiat' |
'osemdesiat' |
'deväťdesiat' |
'dvesto' |
'štyristo' |
'päťsto' |
'šesťsto' |
'sedemsto' |
'osemsto' |
'deväťsto' |
'tisíc' | 
'milión' );


poradieslovom = ('prv' ( [ýáé] |'ej'| 'ého')) |
('druh' ( [ýáéú] | 'ej' | 'ého')) |
('tret' ( [í] | 'ia' | 'ie' | 'ej'| 'iu' | 'ieho')) |
('štvrt' ( [ýáéú] | 'ej' | 'ého')) |
('piat' ( [iyaeu]  | 'ej' | 'eho')) |
('šiest' ( [iyaeu]  | 'ej' | 'eho')) |
('siedm' ( [iyaeu]  | 'ej' | 'eho')) |
('ôsm' ( [iaeu]  | 'ej' | 'eho')) |
('deviat' ( [iyaeu]  | 'ej' | 'eho')) |
('desiat' ( [iyaeu]  | 'ej' | 'eho')) |
('jedenást' ( [iyaeu]  | 'ej' | 'eho')) |
('dvanást' ( [iyaeu]  | 'ej' | 'eho')) |
('trinást' ( [iyaeu]  | 'ej' | 'eho')) |
('štrnást' ( [iyaeu]  | 'ej' | 'eho')) |
('pätnást' ( [iyaeu]  | 'ej' | 'eho')) |
('šestnást' ( [iyaeu]  | 'ej' | 'eho')) |
('sedemnást' ( [iyaeu]  | 'ej' | 'eho')) |
('osemnást' ( [iyaeu]  | 'ej' | 'eho')) |
('devätnást' ( [iyaeu]  | 'ej' | 'eho')) |
('dvadsiat' ( [iyaeu]  | 'ej' | 'eho')) |
('tridsiat' ( [iyaeu]  | 'ej' | 'eho')) |
('štyridsiat' ( [iyaeu]  | 'ej' | 'eho')) |
('päťdesiat' ( [iyaeu]  | 'ej' | 'eho')) |
('šesťdesiat' ( [iyaeu]  | 'ej' | 'eho')) |
('sedemdesiat' ( [iyaeu]  | 'ej' | 'eho')) |
('osemdesiat' ( [iyaeu]  | 'ej' | 'eho')) |
('deväťdesiat' ( [iyaeu]  | 'ej' | 'eho')) |
('st' ( [iýáéú] 'ej' 'ého')) |
('tisíc' ( [iaeu]  | 'ej' | 'eho')) |
('miliónt' ( [yaeu]  | 'ej' | 'eho') ) ;

koncovky = ('ú'| 'á'| 'é' |'ého' |'ej');
 
nasobky = (
'krát' |
'izbov' :>> koncovky |
'poschodov' :>> koncovky |
'násobn' :>> koncovky |
'ročn' :>> koncovky 
);

mesiac = ('január' :>> [ia]?|
'február' :>> [ia]?|
'marc' :>> [ia]?|
'marec'|
'apríl' :>> [ia]?|
'máj' :>> [ia]?|
'jún' :>> [ia]?|
'júl' :>> [ia]?|
'august' :>> [ea]?|
'septembr' :>> [ia]?|
'september' |
'októbr' :>> [ia]? |
'októbrer' |
'novmebr' :>> [ia]?|
'novmeber' :>>[ia]?|
'decembr' :>>[ia]?|
'december' :>> [ia]? );
}%%