package application;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javafx.stage.FileChooser.ExtensionFilter;

public class Controller_MainPage {

	// ----- valeurs envoyées dans la fonction finale :
	private int choixBackfilling; // 0 = non | 1 = oui
	private int choixTri; // 0 = OList | 1 = BSTree | 2 = EBSTree
	private int choixOrdre; // 0 = SPT | 1 = LPT | 2 = WSPT | 3 = FCFS
	private String choixFichier;

	File file;
	FileChooser fileChooser = new FileChooser();
	ObservableList<String> list_tris = FXCollections.observableArrayList("OList", "BSTree", "EBSTree");
	ObservableList<String> list_ordre = FXCollections.observableArrayList("SPT", "LPT", "WSPT", "FCFS");

	@FXML private Button _compute;
	@FXML private Button _file;
	@FXML private ChoiceBox<String> _typeTri;
	@FXML private ChoiceBox<String> _typeOrdre;
	@FXML private RadioButton _yes;
	@FXML private RadioButton _no;
	@FXML private Label _msgFile;
	@FXML private Label _msgTri;
	@FXML private Label _msgOrdre;
	@FXML private Label _msgBackfilling;
	@FXML private Label _makespan;
	@FXML private Label _WjCj;
	@FXML private Label _WjFj;
	@FXML private Label _WjTj;
	@FXML private HBox _Gantt;
	@FXML private HBox _time;

	ToggleGroup backfilling = new ToggleGroup();

	@FXML
	private void initialize() {
		// ----- File -----
		fileChooser.setTitle("Choisir un fichier");
		fileChooser.getExtensionFilters().addAll(new ExtensionFilter("All Files", "*.*"));

		// ----- RadioButton -----
		_yes.setToggleGroup(backfilling);
		_no.setToggleGroup(backfilling);

		// ----- ChoiceBox -----
		_typeTri.setItems(list_tris);
		_typeOrdre.setItems(list_ordre);
	}

	@FXML
	private void fileChooser(ActionEvent event) throws IOException {
		Stage primaryStage = (Stage)((Node) event.getSource()).getScene().getWindow();
		file = fileChooser.showOpenDialog(primaryStage);
		if(file != null) {
			_msgFile.setTextFill(Color.BLACK);
	    	_msgFile.setText(file.toURI().toString());
	    }
	}


	// ========================== FONCTIONS DE COMMUNICATION AVEC C ==========================

	public native void getSchedule(String inFileName, String outFileName, int datastructure, int ordre, int backfilling);


	@FXML
	private void compute(ActionEvent event) throws IOException {
		// ----- confimation choix du fichier :
		if(file != null) {
			setChoixFichier(file.toURI().toString().substring(5));
		}

		// ----- confirmation choix du tri :
		if(_typeTri.getValue() != null) {
			if(_typeTri.getValue() == "OList") {
				setChoixTri(0);
			}
			else if(_typeTri.getValue() == "BSTree") {
				setChoixTri(1);
			}
			else if(_typeTri.getValue() == "EBSTree") {
				setChoixTri(2);
			}
		}

		// ----- confirmation choix de l'ordre :
		if(_typeOrdre.getValue() != null) {
			if(_typeOrdre.getValue() == "SPT") {
				setChoixOrdre(0);
			}
			else if(_typeOrdre.getValue() == "LPT") {
				setChoixOrdre(1);
			}
			else if(_typeOrdre.getValue() == "WSPT") {
				setChoixOrdre(2);
			}
			else if(_typeOrdre.getValue() == "FCFS") {
				setChoixOrdre(3);
			}
		}

		// ----- confirmation choix du backfilling :
		if(backfilling.getSelectedToggle() != null) {
			if(_yes.isSelected()) {
				setChoixBackfilling(1);
			}
			else if(_no.isSelected()){
				setChoixBackfilling(0);
			}
		}

		// ----- confirmation que toutes les options sont cochees :
		if(file == null || _typeTri.getValue() == null || _typeOrdre.getValue() == null || backfilling.getSelectedToggle() == null) {
			if(file == null) {
				_msgFile.setTextFill(Color.RED);
				_msgFile.setText("Choisissez un fichier");
			}
			else {
				_msgFile.setText("");
			}

			if(_typeTri.getValue() == null) {
				_msgTri.setText("Choisissez quel type de tri vous voulez");
			}
			else {
				_msgTri.setText("");
			}

			if(_typeOrdre.getValue() == null) {
				_msgOrdre.setText("Choisissez quel ordre vous voulez");
			}
			else {
				_msgOrdre.setText("");
			}

			if(backfilling.getSelectedToggle() == null) {
				_msgBackfilling.setText("Choisissez si vous voulez du backfilling ou non");
			}
			else {
				_msgBackfilling.setText("");
			}
		}
		else {
			_msgTri.setText("");
			_msgOrdre.setText("");
			_msgBackfilling.setText("");
			
			System.out.println("Appel de la fonction getSchedule(InFileName, OutFileName, datastructure, order, backfilling)");
			System.out.println("");
			System.out.println("  Arguments passes : ");
			System.out.println("---- InFileName : " + getChoixFichier());
			System.out.println("--- OutFileName : out.txt");
			System.out.println("- datastructure : " + getChoixTri());
			System.out.println("--------- order : " + getChoixOrdre());
			System.out.println("--- backfilling : " + getChoixBackfilling());
			System.out.println("");
						
			new Controller_MainPage().getSchedule(getChoixFichier(), "out.txt", getChoixTri(), getChoixOrdre(), getChoixBackfilling());
			visualisation();
		}	
	}
	
	
	private void visualisation() {
		_Gantt.getChildren().clear();
		_time.getChildren().clear();
		int zoom = 15; 
		
		int Pj = 0;
		int Rj = 0;
		int Dj = 0;
		int Wj = 0;
		int Sj = 0;	
		long makespan = 0;
		long WjCj = 0;
		long WjFj = 0;
		long WjTj = 0;
		long oldMakespan = 0;
		
		try {
			File out = new File("out.txt");
			Scanner reader = new Scanner(out);
			String line;
			String [] data;
			
			while(reader.hasNextLine()) {
				line = reader.nextLine();
				data = line.split("\t");
				
				oldMakespan = makespan;
				
				Pj = Integer.parseInt(data[1]);
				Rj = Integer.parseInt(data[2]);
				Dj = Integer.parseInt(data[3]);
				Wj = Integer.parseInt(data[4]);
				Sj = Integer.parseInt(data[5]);	
				
				makespan = Sj + Pj;				
				WjCj += Wj * (Sj + Pj);
				WjFj += Wj * (Sj + Pj - Rj);
				if((Sj + Pj - Dj) > 0) {
					WjTj += Wj * (Sj + Pj - Dj);
				}
				
				StackPane task = new StackPane();
				
				Rectangle blank = new Rectangle();
				blank.setWidth((Sj - oldMakespan) * zoom);
				
				Text text = new Text(data[0]);
				Rectangle rect = new Rectangle();
				rect.setWidth((makespan - Sj) * zoom);
				rect.setHeight(50);
				rect.setFill(Color.color(Math.random(), Math.random(), Math.random()));
				
				task.getChildren().addAll(rect, text);
				_Gantt.getChildren().add(blank);
				_Gantt.getChildren().add(task);	
				
				
				Rectangle blank2 = new Rectangle();
				blank2.setWidth((Sj - oldMakespan) * zoom);		
				
				Text start = new Text("|"+Sj);
				Rectangle space = new Rectangle();
				space.setWidth( ((makespan - Sj) * zoom) - start.getLayoutBounds().getWidth() );

				_time.getChildren().add(blank2);
				_time.getChildren().add(start);	
				_time.getChildren().add(space);	
			}
			reader.close();			
		}
		catch(FileNotFoundException e) {
			e.printStackTrace();
		}
		
		_makespan.setText(""+makespan);
		_WjCj.setText(""+WjCj);
		_WjFj.setText(""+WjFj);
		_WjTj.setText(""+WjTj);
	}



	// ========================== GETTERS / SETTERS ==========================

	public int getChoixBackfilling() {
		return choixBackfilling;
	}
	public void setChoixBackfilling(int choixBackfilling) {
		this.choixBackfilling = choixBackfilling;
	}

	public int getChoixTri() {
		return choixTri;
	}
	public void setChoixTri(int choixTri) {
		this.choixTri = choixTri;
	}

	public int getChoixOrdre() {
		return choixOrdre;
	}
	public void setChoixOrdre(int choixOrdre) {
		this.choixOrdre = choixOrdre;
	}

	public String getChoixFichier() {
		return choixFichier;
	}
	public void setChoixFichier(String choixFichier) {
		this.choixFichier = choixFichier;
	}
}
