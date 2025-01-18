import {Component} from "@angular/core";
import {ThemeService} from "./service/theme.service";
import {MatButtonModule} from "@angular/material/button";
import {MatIconModule} from "@angular/material/icon";

@Component({
	selector: "app-root",
	imports: [
		MatButtonModule,
		MatIconModule,
	],
	templateUrl: "./app.component.html",
	styleUrls: ["./app.component.css"],
})
export class AppComponent {

	constructor(private readonly themeService: ThemeService) {
	}

	isDarkTheme() {
		return this.themeService.isDarkTheme();
	}

	setTheme(isDarkTheme: boolean) {
		this.themeService.setTheme(isDarkTheme);
	}
}
