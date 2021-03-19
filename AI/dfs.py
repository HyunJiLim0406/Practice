import time
# 상태를 나타내는 클래스
class State:
  def __init__(self, board, goal, moves=0):
    self.board = board
    self.moves = moves
    self.goal = goal

  # i1과 i2를 교환하여 새로운 상태를 반환한다. 
  def get_new_board(self, i1, i2, moves):
    new_board = self.board[:]
    new_board[i1], new_board[i2] = new_board[i2], new_board[i1]
    return State(new_board, self.goal, moves)

  # 자식 노드를 확장하여 리스트에 저장 후 반환한다.
  # up/left/down/right 의 결과를 stack처럼 저장한다.
  def expand(self, moves):
    result = []
    i = self.board.index(0)		# 숫자 0(빈칸)의 위치를 찾는다.
    
    if not i in [0, 1, 2] :		# UP 연산자
      #print("up")
      result.append(self.get_new_board(i, i-3, moves))
    if not i in [0, 3, 6] :		# LEFT 연산자 
      #print("left")
      result.append(self.get_new_board(i, i-1, moves))
    if not i in [6, 7, 8]:		# DOWN 연산자
      #print("down")
      result.append(self.get_new_board(i, i+3, moves))
    if not i in [2, 5, 8]:		# RIHT 연산자
      #print("right")
      result.append(self.get_new_board(i, i+1, moves))  

    return result
  

  # 객체를 출력할 때 사용한다. 
  def __str__(self):
    return str(self.board[:3]) +"\n"+\
    str(self.board[3:6]) +"\n"+\
    str(self.board[6:]) +"\n"+\
    "------------------"
 
  
  def __eq__(self, other):
    return self.board == other.board

#초기 상태
puzzle = [1, 2, 3, 
          4, 0, 5, 
          7, 8, 6]
# 목표 상태
goal = [1, 2, 3, 
        4, 5, 6, 
        7, 8, 0]
#test   : bfs, a*와 비교해 볼 
#goal = [1,3,5,4,2,0,7,8,6]

start = time.time()

# open 리스트
open_list = [ ]
open_list.append(State(puzzle, goal))
print("------------")

closed = [ ]
moves = 0

#  디버깅을 위한 코드
#print("START OF OPENQ")
#for elem in open_list:
#  print(elem)
#print("END OF OPENQ")

i=0
while len(open_list) != 0:
  
  # OPEN 리스트의 앞에서 pop : up/left/down/right 순으로 저장되어 right 가 pop
  current = open_list.pop(0)
  
  i+=1
  print("##",i)
  print(current)
  
  if current.board == goal:
      print("탐색 성공")
      break
  moves = current.moves+1
  closed.append(current)
  
  
  for state in current.expand(moves):
      # 이미 거쳐간 노드이면  다음 노드로.
      if (state in closed) or (state in open_list):	
          continue				
      else: 
          # bfs에서는 OPEN 리스트의 끝에 추가했지만, dfs에서는 맨 앞에 추가합니다. 
          # open_list.append(state)  대신, 	
          open_list.insert(0,state)
          


print("dfs count : " , i)
print("dfs 소요시간 : " , time.time()-start)
