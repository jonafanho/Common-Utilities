import {Component} from "@angular/core";
import {MatButtonModule} from "@angular/material/button";
import {MatIconModule} from "@angular/material/icon";
import {ThemeService} from "../../service/theme.service";

@Component({
	selector: "app-main",
	imports: [
		MatButtonModule,
		MatIconModule,
	],
	templateUrl: "./main.component.html",
	styleUrls: ["./main.component.css"],
})
export class MainComponent {

	constructor(private readonly themeService: ThemeService) {
	}

	isDarkTheme() {
		return this.themeService.isDarkTheme();
	}

	setTheme(isDarkTheme: boolean) {
		this.themeService.setTheme(isDarkTheme);
	}
}
