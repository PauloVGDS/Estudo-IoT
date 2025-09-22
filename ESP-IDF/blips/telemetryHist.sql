select tele.time, tele.measure_name, if(tele.measure_value::varchar is not null, tele.measure_value::varchar, cast(tele.measure_value::bigint as varchar)) as measure_value
from "NeuronioDB"."MQTT_Tele" tele
where id = 'blips_CD8DA2'
/*and time >= ago(90d)*/
and measure_name = 'Mem2'
order by tele.time desc