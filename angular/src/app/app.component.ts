import {Component} from "@angular/core";
import {MainComponent} from "./component/main/main.component";

@Component({
	selector: "app-root",
	imports: [
		MainComponent,
	],
	templateUrl: "./app.component.html",
	styleUrls: ["./app.component.css"],
})
export class AppComponent {

	constructor() {
	}
}
