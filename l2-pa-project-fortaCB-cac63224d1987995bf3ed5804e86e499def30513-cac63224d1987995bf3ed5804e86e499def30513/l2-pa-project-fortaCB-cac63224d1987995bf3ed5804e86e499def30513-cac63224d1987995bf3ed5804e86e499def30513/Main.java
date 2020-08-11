import java.util.*;

public class Main {
	// verifica daca s-a primit o mutare valida ca input
	public static int isMove(String s, Board b) {
		if (s.length() > 5)
			return 0;
		if (s.charAt(0) < 'a' || s.charAt(0) > 'h')
			return 0;
		if (s.charAt(1) < '1' || s.charAt(1) > '8')
			return 0;
		if (s.charAt(2) < 'a' || s.charAt(2) > 'h')
			return 0;
		if (s.charAt(3) < '1' || s.charAt(3) > '8')
			return 0;
		return 1;
	}
	/**
	 * Verifica daca exista mutari realizabile de pioni
	 * @param b variabila de tip Board necesara pentru accesarea tablei si functiilor
	 * @param mode culoarea
	 * @return 0 daca nu exista mutari valabile, 1 daca exista
	 */
	//Nu este folosita in etapa aceasta a proiectului
	public static int makeAPMove(Board b, String mode) {
		String s = b.pawnMove(mode);
		// daca functia a intors "no moves" nu mai sunt mutari valide pentru pioni 
		if (s.contains("no")) 
			return 0;
		else {
			System.out.println("move " + s);
			b.move(s);
		}
		return 1;
	}

	// Verifica daca se poate face o mutare valida a nebunului
	// Similar cu makeAPMove ca parametri si return
	//Nu este folosita in etapa aceasta a proiectului
	public static int makeABMove(Board b, String mode) {
		String s = b.bishopMove(mode);
		if (s.contains("no"))
			return 0;
		else {
			System.out.println("move " + s);
			b.move(s);
		}
		return 1;
	}

	// verifica daca regina poate face miscari valide pe tabla
	// Similar cu makeAPMove ca parametri si return
	//Nu este folosita in etapa aceasta a proiectului
	public static int makeAQMove(Board b, String mode) {
		String s = b.queenMove(mode);
		if (s.contains("no"))
			return 0;
		else {
			System.out.println("move " + s);
			b.move(s);
		}
		return 1;
	}

	// verifica daca se poate face o miscare valida pe tabla cu tura
	// Similar cu makeAPMove ca parametri si return
	//Nu este folosita in etapa aceasta a proiectului
	public static int makeARMove(Board b, String mode) {
		String s = b.rookMove(mode);
		if (s.contains("no"))
			return 0;
		else {
			System.out.println("move " + s);
			b.move(s);
		}
		return 1;
	}

	// Verifica daca se poate face o mutare valida cu calul
	// Similar cu makeAPMove ca parametri si return
	//Nu este folosita in etapa aceasta a proiectului
	public static int makeAKMove(Board b, String mode) {
		String s = b.knightMove(mode);
		if (s.contains("no")) 
			System.out.println("resign");
		else {
			System.out.println("move " + s);
			b.move(s);
		}
		return 1;
	}
	// Verifica daca se poate face o mutare valida cu regele
	// Similar cu makeAPMove ca parametri si return
	public static int makeAKingMove(Board b, String mode) {
		String s = b.kingMove(mode);
		if (s.contains("no"))
			return 0;
		else {
			System.out.println("move " + s);
			b.move(s);
		}
		return 1;
	}
	// Verifica miscarile tuturor tipurilor de piese pe rand
	// Se introduc intr-o lista toate miscarile de la fiecare piese si se apeleaza metoda makeMove din clasa Board
	// Aceasta metoda intoarce cea mai buna mutare dintre cele introduse in lista
	public static int makeAMove(Board b, String mode) {
		String s;
		// Se creeaza o lista cu cele mai bune miscari de la fiecare piesa
		ArrayList<String> l = new ArrayList<>();
		// Se verifica miscarile pionilor
		s = b.pawnMove(mode);
		if (!s.contains("no"))
			l.add(s);
		//Se verifica miscarile regelui
		s = b.kingMove(mode);
		if (!s.contains("no"))
			l.add(s);
		//Se verifica miscarile reginei
		s = b.queenMove(mode);
		if (!s.contains("no"))
			l.add(s);
		//Se verifica miscarile turei
		s = b.rookMove(mode);
		if (!s.contains("no"))
			l.add(s);
		//Se verifica miscarile nebunului
		s = b.bishopMove(mode);
		if (!s.contains("no"))
			l.add(s);
		//Se verifica miscarile calului
		s = b.knightMove(mode);
		if (!s.contains("no"))
			l.add(s);
		//Se aplica functia makeMove pentru a intoarce cea mai buna mutare dintre toate
		s = b.makeMove(l, mode);
		//Daca nici o piesa nu poate realiza o mutare se da resign 
		if (s.contains("no")) {
			System.out.println("resign");
			return 0;
		}
		//Se afiseaza mutarea si se realizeaza mutarea in matrice folosing functia move
		System.out.println("move " + s);
		b.move(s);
		return 1;
	}

	public static void main(String[] args) throws InterruptedException {

		// TODO Auto-generated method stub
		// Se citesc comenzile de la tastatura. Xboard-ul da comenzi la Stdin 
		Scanner sc = new Scanner(System.in);
		// Culoarea este stabilita automat ca negru.
		String line,  mode = "black";
		// Initializez tabla si jocul
		Board b = new Board();

		while (true) {
			//Se citeste cate o linie pe rand
			line = sc.nextLine();

			// Ignoram comenzile specifice xboardului care nu afecteaza engine-ul
			if (line.contains("post"))
				continue;
			if (line.contains("hard"))
				continue;
			if (line.contains("easy"))
				continue;
			if (line.contains("time"))
				continue;
			if (line.contains("otim"))
				continue;
			if (line.contains("undo"))
				continue;

			// Dezactivam modul care trimite SIGINT
			if (line.contains("protover")) {
				System.out.println("feature sigint=0");
			}
			// Se verifica daca se primeste comanda new si se reface tabla la valorile intiale cu constructorul
			if (line.contains("new")) {
				b = new Board();
				mode = "black";
				continue;
			}

			// Se seteaza culoare cu care se joaca in functie de comanda primita si se citeste urmatoare linie
			if (line.contains("black")) {
				mode = "black";
				line = sc.nextLine();
			}
			if (line.contains("white")) {
				mode = "white";
				line = sc.nextLine();
			}
			// Daca se primeste comanda go se porneste engine-ul
			// Daca regele este amenintat se verifica daca poate fi mutat. Daca regele nu poate fi mutat se da resign
			// Altfel se verifica miscarile tuturor pieselor
			if (line.contains("go")) {
				if (b.verifyKing(mode)) {
					if (makeAKingMove(b, mode) == 0)
						System.out.println("resign");
					System.out.println("yes");
				} else
					makeAMove(b, mode);
				continue;
			} 
			// Se verifica daca linia citita este o mutare valida
			if (isMove(line, b) == 1) {
				b.move(line);
				// Dupa ce se realizeaza o miscare primita ca input, engine-ul realizeaza o mutare
				// Se verifica daca regele este amenintat. Daca regele nu poate fi mutat se da resign
				if (b.verifyKing(mode)) {
					System.out.println(mode + " in check");
					if (makeAKingMove(b, mode) == 0)
						System.out.println("resign");
				} else
					makeAMove(b, mode);
				continue;
			}
			// Daca se intra in modul force engine ul nu mai face mutari singur
			if (line.contains("force")) { 
				while (true) {
					line = sc.nextLine();
					// Se astepta "go", "new" sau un mod de joc(culoare) pentru a iesi din force mod
					if (line.contains("black") || line.contains("white")) { 													
						mode = line;
						break;
					}
					if (line.contains("go"))
						break;
					if (line.contains("new")) {
						b = new Board();
						mode = "black";
						break;
					}

					// Ignoram comenzile specifice xboardului care nu afecteaza engine-ul
					if (line.contains("post"))
						continue;
					if (line.contains("hard"))
						continue;
					if (line.contains("easy"))
						continue;
					if (isMove(line, b) == 1) {
						b.move(line);
						continue;
					}
				}
			}

			if (line.contains("quit"))
				break;
		}
		sc.close();
	}
}