# MakeCache

### FIFO, LRU cache 제작

   
1. FIFO
- 데이터가 캐시에 존재하면 성공,    
데이터가 캐시에 존재하지 않으면 가장 오래전에 입력된 데이터를 캐시에서 삭제하고 방금 존재하지 않았던 값 입력

2. LRU
- 데이터가 캐시에 존재하면 참조한 데이터를 제일 최근 데이터로 옮기고 성공,
데이터가 캐시에 존재하지 않으면 가장 오래전에 참조된 데이터를 캐시에서 삭제하고 방금 존재하지 않았던 값 입력

   
<h5>데이터 참조 성공시 success를 높이고 데이터 참조 실패시 failed를 높여 hit ratio 계산</h5>
