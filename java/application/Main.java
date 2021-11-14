package application;
	
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;
import javafx.scene.Parent;
import javafx.scene.Scene;


public class Main extends Application {
	
	@Override
	public void start(Stage primaryStage) throws Exception {
		try {			
			Parent root = FXMLLoader.load(getClass().getResource("/application/MainPage.fxml"));
		   
			Scene scene = new Scene(root, 600, 420);
			scene.getStylesheets().add(getClass().getResource("application.css").toExternalForm());
			primaryStage.setScene(scene);
			primaryStage.setTitle("Interface scheduling");

			primaryStage.show();
		} 
		catch(Exception e) {
			e.printStackTrace();
		}
	} 
		
	public static void main(String[] args) {
		launch(args);
	}
	static {
		System.loadLibrary("mylibrary");
	}
}