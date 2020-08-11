import java.util.TreeMap;
import java.util.ArrayList;

public class Board {
	//Tabla de sah reprezentata ca o matrice de int-uri
	private int[][] b;
	//Contructorul care initializeaza tabla cu numerele specifice fiecarei piese
	public Board() {
		b = new int[8][8];
		for(int i=2;i<6;i++)
			for(int j=0;j<8;j++) {
				b[i][j]	= 0;
			}
		for(int i=0;i<8;i++) {
			b[6][i] = -1;
			b[1][i] = 1;
		}
		b[0][0] = 4;
		b[0][7] = 4;
		b[7][0] = -4;
		b[7][7] = -4;
		b[0][1] = 3;
		b[0][6] = 3;
		b[7][1] = -3;
		b[7][6] = -3;
		b[0][2] = 2;
		b[0][5] = 2;
		b[7][2] = -2;
		b[7][5] = -2;
		b[0][3] = 5;
		b[7][3] = -5;
		b[0][4] = 6;
		b[7][4] = -6;
	}
	//Afisare a intregi table, folosita mai mult pentru debug
	public void af() {
		for(int i=0;i<8;i++)
			System.out.print( (char)(i+'A')+ " ");
		System.out.println();
		for(int i=0;i<8;i++) {
			for(int j=0;j<8;j++) 
				System.out.print(b[7-i][j] + " ");
			System.out.println();
		}
	}
	//Metoda care realizeaza o mutare pe tabla de joc in functie de Stringul cu mutarea primit 
	public int move(String string) {
		int j=string.charAt(0)-'a';
		int i=string.charAt(1)-'1';
		int newj=string.charAt(2)-'a';
		int newi=string.charAt(3)-'1';
		//Se verifica daca stringul are si optiunea de transformare in regina si se transforma pionul in regina
		if (string.length() == 5 && string.charAt(4) == 'q')
			b[i][j] *= 5;
		b[newi][newj] = b[i][j];
		b[i][j] = 0;
		return 1;
	}
    /**
     * Metoda care intoarce miscarea maxima pe care o poate realiza o regina, un nebun sau o tura
     * @param i linia din matrice
     * @param j coloana din matrice
     * @param sgn tipul piesei(-1 negru, 1 alb)
     * @return un string ce reprezinta mutarea ce trebuie facuta in matrice
     */
    public String moveAtDistance(int i,int j,int sgn) {
    	//Miscarile se memoreaza intr-un map si la final este intoarsa cea mai buna mutare
    	TreeMap<Integer,String> map=new TreeMap<Integer,String>();
    	for(int oi=-1;oi<=1;oi++) 
			for(int oj=-1;oj<=1;oj++) {
				//daca e regina verifica toate pozitile mai putin pozitia pe care se afla acum
				if(b[i][j]==sgn*6 && (oi==oj && oi==0)) continue;
				//daca e tura verifica doar pozitiile din laterale, nu si diagonalele
				if(b[i][j]==sgn*4 && (Math.abs(oi)==Math.abs(oj))) continue;
				//daca e nebun verifica doar diagonalele nu si lateralele
				if(b[i][j]==sgn*2 && (oi==0 || oj == 0)) continue;
				int auxi=oi, auxj=oj;
				//cauta miscarea la distanta mare
				if(i+auxi<=7 && i+auxi>=0 && j+auxj<=7 && j+auxj>=0 && b[i+auxi][j+auxj]*sgn == 0) {
					while(i+auxi<=7 && i+auxi>=0 && j+auxj<=7 && j+auxj>=0) {
						if(b[i+auxi][j+auxj]*sgn>0) break;
						auxi+=oi;
						auxj+=oj;
					}
					auxi-=oi;
					auxj-=oj;
					map.put(b[i+auxi][j+auxj]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j+auxj) + (i+1+auxi)));
				}
		}
    	if(map.isEmpty()) return "no";
    	return map.get(((TreeMap<Integer, String>) map).firstKey());	
    }
    //Metoda care intoarce miscarea cea mai buna, care poate lua o piesa adversa, cu o regina, un nebun sau o tura
    //Este similara cu moveAtDistance dar memoreaza doar miscarile care ar putea lua o piesa adversa 
    public String killAtDistance(int i,int j,int sgn) {
    	TreeMap<Integer,String> map=new TreeMap<Integer,String>();
    	for(int oi=-1;oi<=1;oi++) 
			for(int oj=-1;oj<=1;oj++) {
				if(b[i][j]==sgn*6 && (oi==oj && oi==0)) continue;
				if(b[i][j]==sgn*4 && (Math.abs(oi)==Math.abs(oj))) continue;
				if(b[i][j]==sgn*2 && (oi==0 || oj==0)) continue;
				int auxi=oi, auxj=oj;
				while(i+auxi<=7 && i+auxi>=0 && j+auxj<=7 && j+auxj>=0) {
					if(b[i+auxi][j+auxj]*sgn>0) break;
					if(b[i+auxi][j+auxj]*sgn<0) {
						map.put(b[i+auxi][j+auxj]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j+auxj) + (i+1+auxi)));
						break;
					}
					auxi+=oi;
					auxj+=oj;
				}
		}

    	if(map.isEmpty()) return "no";
    	//Este returnata piesa care are cel mai mare numar/scor in modul
    	return map.get(((TreeMap<Integer, String>) map).firstKey());	
    }
    /**
     * Metoda care verifica daca regele poate lua o piesa adversa sau se poate misca si intoarce acea miscare
     * @param i linia din matrice corespunzatoare regelui
     * @param j	coloana din matrice corespunzatoare regelui
     * @param sgn semnul piesei(-1 negru, +1 alb)
     * @return cea mai buna miscare posibila sau "no moves" daca nu exista miscari posibile
     */
    public String killAsAKing(int i,int j,int sgn) {
    	TreeMap <Integer,String> map=new TreeMap<Integer, String>();
    	if(i<7 && b[i+1][j]*sgn<=0 && !danger(i+1,j,sgn)) map.put(b[i+1][j]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j) + (i+2)));
    	if(i>0 && b[i-1][j]*sgn<=0 && !danger(i-1,j,sgn)) map.put(b[i-1][j]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j) + i));
		if(i<7 && j<7 && b[i+1][j+1]*sgn<=0 && !danger(i+1,j+1,sgn)) map.put(b[i+1][j+1]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j+1) + (i+2)));
		if(i<7 &&  j>0 && b[i+1][j-1]*sgn<=0 && !danger(i+1,j-1,sgn)) map.put(b[i+1][j-1]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j-1) + (i+2)));
		if(j<7 && b[i][j+1]*sgn<=0 && !danger(i,j+1,sgn)) map.put(b[i][j+1]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j+1) + (i+1)));
		if(j>0 && b[i][j-1]*sgn<=0 && !danger(i,j-1,sgn)) map.put(b[i][j-1]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j-1) + (i+1)));
		if(i>0 && j<7 && b[i-1][j+1]*sgn<=0 && !danger(i-1,j+1,sgn)) map.put(b[i-1][j+1]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j+1) + i));
		if(i>0 && j>0 && b[i-1][j-1]*sgn<=0 && !danger(i-1,j-1,sgn)) map.put(b[i-1][j-1]*sgn,("" + (char)('a'+j) + (i+1) + (char)('a'+j-1) + i));

		if(map.isEmpty()) 
    		return "no moves";
    	return map.get(map.firstKey());
    }
    /**
     * Metoda care verifica daca calul poate lua o piesa adversa si intoarce acea miscare
     * @param i linia din matrice corespunzatoare calului
     * @param j coloana din matrice corespunzatoare calului
     * @param sgn semnul piesei(-1 negru,+1 alb)
     * @return cea mai buna miscare posibila sau "no moves" daca nu exista miscari posibile
     */
    public String killAsKnight(int i,int j,int sgn) {
    	TreeMap<Integer, String> map=new TreeMap<>();
    	if(i-1>0 && j<7 && b[i-2][j+1]*sgn<=0) map.put(b[i-2][j+1]*sgn, (""+(char)('a'+j)+(i+1)+(char)('a'+j+1)+(i-1)));
		if(i-1>0 && j>0 && b[i-2][j-1]*sgn<=0) map.put(b[i-2][j-1]*sgn,(""+(char)('a'+j)+(i+1)+(char)('a'+j-1)+(i-1)));
		if(i+1<7 && j<7 && b[i+2][j+1]*sgn<=0) map.put(b[i+2][j+1]*sgn,(""+(char)('a'+j)+(i+1)+(char)('a'+j+1)+(i+3)));
		if(i+1<7 && j>0 && b[i+2][j-1]*sgn<=0) map.put(b[i+2][j-1]*sgn,(""+(char)('a'+j)+(i+1)+(char)('a'+j-1)+(i+3)));
		if(i>0 && j-1>0 && b[i-1][j-2]*sgn<=0) map.put(b[i-1][j-2]*sgn,(""+(char)('a'+j)+(i+1)+(char)('a'+j-2)+(i)));
		if(i>0 && j+1<7 && b[i-1][j+2]*sgn<=0) map.put(b[i-1][j+2]*sgn,(""+(char)('a'+j)+(i+1)+(char)('a'+j+2)+(i)));
		if(i<7 && j-1>0 && b[i+1][j-2]*sgn<=0) map.put(b[i+1][j-2]*sgn,(""+(char)('a'+j)+(i+1)+(char)('a'+j-2)+(i+2)));
		if(i<7 && j+1<7 && b[i+1][j+2]*sgn<=0) map.put(b[i+1][j+2]*sgn,(""+(char)('a'+j)+(i+1)+(char)('a'+j+2)+(i+2)));

		if(map.isEmpty())
	    	return "no moves";
	    return map.get(map.firstKey());
    }
    //Metoda care cauta pionii pe tabla si face o miscare sau ia o piesa adversa
    public String pawnMove(String mode) {
    	int i, j, sgn = 1;
    	if (mode.contains("black")) sgn = -1;
    	TreeMap<Integer,String> map=new TreeMap<>();
    	for (j = 0; j < 8; j++) 
    		for (i = 1; i < 7; i++)
    			if(b[i][j] == sgn) {  //caut pioni pe toata tabla 
    				if (j > 0 && b[i+sgn][j-1]*sgn < 0) {//verific daca pe pozitia inainte stanga este o piesa a adversarului pe care o pot lua;			
    					if((i==1 && sgn==-1) || (i==6 && sgn==1))
    						map.put((b[i+sgn][j-1]-sgn)*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j-1) + (i+1+sgn))+"q");
						map.put(b[i+sgn][j-1]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j-1) + (i+1+sgn)));
    				}
					if(j<7 && b[i+sgn][j+1]*sgn < 0) {//verific daca pe pozitia inainte dreapta este o piesa a adversarului pe care o pot lua;	
						if((i==1 && sgn==-1) || (i==6 && sgn==1))
							map.put((b[i+sgn][j+1]-sgn)*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j+1) + (i+1+sgn))+"q");
						map.put(b[i+sgn][j+1]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j+1) + (i+1+sgn)));
					}
					if (b[i+sgn][j] == 0) { //verific daca pot inainta 
							if((i==1 && sgn==-1) || (i==6 && sgn==1))
								map.put((b[i+sgn][j]-sgn)*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j) + (i+1+sgn))+"q");
							map.put(b[i+sgn][j]*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j) + (i+1+sgn)));
							if((i==1 && sgn==1) || (i==6 && sgn==-1))// verific daca pionul este pe pozitia initiala si poate avansa 2 patrate
								if(b[i+2*sgn][j] == 0)
									map.put((b[i+2*sgn][j]-sgn)*sgn, ("" + (char)('a'+j) + (i+1) + (char)('a'+j) + (i+1+2*sgn)));
					}
    			}
    	System.out.println(map);
    	if(map.isEmpty())
    		return "no moves";
    	return map.get(map.firstKey());
    }
    //Metoda care cauta o regina pe tabla si face o miscare cu acea piesa
    public String queenMove(String mode) {
    	int i, j, sgn = 1;
    	if (mode.contains("black")) sgn = -1;
    	for(j=0;j<8;j++)
    		for(i=0;i<8;i++)
    			if(b[i][j]==sgn*5) {
    				String s = killAtDistance(i,j,sgn);
					if(s.contains("no")==false)return s;
					s = moveAtDistance(i,j,sgn);
					if(s.contains("no")==false)return s;
    			}
    	return "no moves";
    }
    //Metoda care cauta un nebun pe tabla si face o miscare cu acea piesa
    public String bishopMove(String mode) {
    	int i, j, sgn = 1;
    	if (mode.contains("black")) sgn = -1;
    	for(j=0;j<8;j++)
    		for(i=0;i<8;i++)
    			if(b[i][j]==sgn*2) {
    				String s = killAtDistance(i,j,sgn);
					if(s.contains("no")==false)return s;
					s = moveAtDistance(i,j,sgn);
					if(s.contains("no")==false)return s;
    			}
    	return "no moves";
    }
   //Metoda care cauta o tura pe tabla si face o miscare cu acea piesa   
    public String rookMove(String mode) {
    	int i, j, sgn = 1;
    	if (mode.contains("black")) sgn = -1;
    	for(j=0;j<8;j++)
    		for(i=0;i<8;i++)
    			if(b[i][j]==sgn*4) {
    				String s = killAtDistance(i,j,sgn);
					if(s.contains("no")==false)return s;
					s = moveAtDistance(i,j,sgn);
					if(s.contains("no")==false)return s;
    			}
    	return "no moves";
    }
    //Metoda care cauta un cal pe tabla si face o miscare cu acea piesa
    public String knightMove(String mode) {
    	int i,j, sgn = 1;
    	if (mode.contains("black")) sgn = -1;
    	for(j=0;j<8;j++)
    		for(i=0;i<8;i++)
    			if(b[i][j]==sgn*3) {
    				return killAsKnight(i,j,sgn);
    			}
    	return "no moves";
    }
    //Metoda care cauta un rege si intoarce o miscare
    public String kingMove(String mode) {
    	int i, j, sgn = 1;
    	if(mode.contains("black")) sgn = -1;
    	for(j=0;j<8;j++)
    		for(i=0;i<8;i++)
    			if(b[i][j]==sgn*6) {
    				return killAsAKing(i,j,sgn);
    			}
    	return "no moves";
    }
    /**
     * Metoda care verifica mutarile posibile ale fiecarui tip de piesa si o face pe cea mai profitabila
     * @param l lista cu toate mutarile posibile la momentul apelarii
     * @param mode culoarea pieselor
     * @return cea mai buna mutare din toate posibile
     */
    public String makeMove(ArrayList<String> l, String mode) {
    	TreeMap<Double, String> map=new TreeMap<>();
    	int sgn = 1;
    	if (mode.contains("black"))
    		sgn = -1;
    	
    	for(String s: l) {
    		if (b[s.charAt(1) - '1'][s.charAt(0) - 'a'] == sgn)				// verific daca e pion 
    			map.put( (b[s.charAt(3) - '1'][s.charAt(2) - 'a']*sgn)-0.1, s); //prefer sa mute pionul in fata decat sa mute tura inainte inapoi daca nu poate omori nimic
    		else
    			map.put((double)b[s.charAt(3) - '1'][s.charAt(2) - 'a']*sgn, s);
    	}
    	
    	if(map.isEmpty())
	    	return "no moves";
	    return map.get(map.firstKey());
    } 
    /**
     * Verifica daca regele este in sah in functie de caii adversi
     * @param i linia din matrice corespunzatoare regelui 
     * @param j coloana din matrice corespunzatoare regelui
     * @param sgn semnul regelui (-1 negru, +1 alb)
     * @return Daca exista un cal care ameninta regele sau nu
     */
    public boolean checkKnight(int i, int j, int sgn) {
    	if(i-1>0 && j<7 && b[i-2][j+1]*sgn==-3) return true;
		if(i-1>0 && j>0 && b[i-2][j-1]*sgn==-3) return true;
		if(i+1<7 && j<7 && b[i+2][j+1]*sgn==-3) return true;
		if(i+1<7 && j>0 && b[i+2][j-1]*sgn==-3) return true;
		if(i>0 && j-1>0 && b[i-1][j-2]*sgn==-3) return true;
		if(i>0 && j+1<7 && b[i-1][j+2]*sgn==-3) return true;
		if(i<7 && j-1>0 && b[i+1][j-2]*sgn==-3) return true;
		if(i<7 && j+1<7 && b[i+1][j+2]*sgn==-3) return true;
    	
    	return false;
    }
    /**
     * Verifica daca regele este in sah in functie de toate piesele
     * @param i linia din matrice corespunzatoare regelui
     * @param j coloana din matrice corespunzatoare regelui
     * @param sgn semnul regelui (-1 negru, +1 alb)
     * @return Daca exista vreo piesa care ameninta regele sau nu
     */
    public boolean danger(int i,int j,int sgn) {
    	int oi, oj, a, c = 0, status = 0;
    	for (a = 0; a < 8; a++) {
    		for (c = 0; c < 8; c++) 
    			if(b[a][c] == sgn*6) {
    				b[a][c] = 0; 
    				break;
    			}
    		if(c<8) break;
    	}
    	// Se verifica fiecare pozitie din jurul regelui
    	for(oi=-1;oi<=1;oi++)
    		for(oj=-1;oj<=1;oj++) {
    			if((oi==oj && oi==0)) continue;
    			int auxi=oi, auxj=oj;
    			if (i+oi<=7 && i+oi>=0 && j+oj<=7 && j+oj>=0 && b[i+oi][j+oj]==(-6)*sgn) status = 1; // verificam daca e regele advers langa
				while(i+auxi<=7 && i+auxi>=0 && j+auxj<=7 && j+auxj>=0) {
					//Se verifica daca pe diagonale exista regine adverse sau nebuni adversi
					if(Math.abs(auxi)==Math.abs(auxj) && (b[i+auxi][j+auxj]*sgn==-5 || b[i+auxi][j+auxj]*sgn==-2)) {
						System.out.println("0"+(Math.abs(auxi)==Math.abs(auxj) && b[i+auxi][j+auxj]*sgn==-5)+i+j+" "+auxi+auxj);
						status = 1;
						break;
					}
					//Se verifica daca pe linie sau coloana exista ture sau regine adverse
					if((auxi==0 || auxj==0) && (b[i+auxi][j+auxj]*sgn==-5 || b[i+auxi][j+auxj]*sgn==-4)) {
						System.out.println("1"+((auxi==0 || auxj==0) && b[i+auxi][j+auxj]*sgn==-5)+i+j+" "+auxi+auxj);
						status = 1;
						break;
					}
					if(b[i+auxi][j+auxj]*sgn != 0) break;
					auxi+=oi;
					auxj+=oj;
				}
    		}
    		//Se verifica daca exista pioni adversi in stanga sau dreapta
    		if( (i+sgn<=7 && i+sgn>=0) && ((j<7 && b[i+sgn][j+1]==sgn*(-1)) || (i>0 && b[i+sgn][j-1]==sgn*(-1))) )
    			status = 1;
    		b[a][c] = sgn*6;
    		//Se verifica daca exista vreun cal care ameninta regele
    		if( status ==1 || checkKnight(i,j,sgn)) {
    			return true;
    		}
    		return false;
    }
    //Verifica daca regele este in sah
    public boolean verifyKing(String mode) {
    	int i, j, sgn = 1;
    	if(mode.contains("black")) sgn = -1;
    	for(j=0;j<8;j++)
    		for(i=0;i<8;i++)
    			if(b[i][j]==sgn*6) {
    				return danger(i,j,sgn);
    			}
    	return false;
    }

}
