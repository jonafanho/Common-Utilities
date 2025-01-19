import {Component} from "@angular/core";
import {MatInputModule} from "@angular/material/input";
import {MatButtonModule} from "@angular/material/button";
import {FormControl, FormGroup, ReactiveFormsModule} from "@angular/forms";
import {HttpClient} from "@angular/common/http";
import {MatProgressSpinnerModule} from "@angular/material/progress-spinner";
import {catchError, EMPTY} from "rxjs";
import {MainComponent} from "./component/main/main.component";
import {ThemeService} from "./service/theme.service";
import {MatSlideToggleModule} from "@angular/material/slide-toggle";

@Component({
	selector: "app-root",
	imports: [
		ReactiveFormsModule,
		MatInputModule,
		MatButtonModule,
		MatProgressSpinnerModule,
		MatSlideToggleModule,
		MainComponent,
	],
	templateUrl: "./app.component.html",
	styleUrls: ["./app.component.css"],
})
export class AppComponent {
	protected status: "main" | "none" | "success" | "fail" = "main";
	protected loading = true;
	protected oldSsid = "";
	protected oldPassword = "";
	protected readonly formGroup;

	constructor(private readonly httpClient: HttpClient, private readonly themeService: ThemeService) {
		this.formGroup = new FormGroup({
			ssid: new FormControl(""),
			password: new FormControl(""),
		});

		this.httpClient.get<{ ssid: string, password: string }>("/api/status").pipe(catchError(() => {
			this.setStatus("fail");
			return EMPTY;
		})).subscribe(({ssid, password}) => {
			if (ssid === undefined || password === undefined) {
				this.setStatus("main");
			} else {
				this.formGroup.get("ssid")?.setValue(ssid);
				this.formGroup.get("password")?.setValue(password);
				this.setStatus("none");
			}
		});

		themeService.isDarkTheme();
	}

	save() {
		const ssid = this.formGroup.getRawValue().ssid;
		const password = this.formGroup.getRawValue().password;

		if (ssid && password) {
			this.loading = true;
			this.httpClient.get<{ success: boolean }>(`/api/save?ssid=${(encodeURIComponent(ssid))}&password=${(encodeURIComponent(password))}`).pipe(catchError(() => {
				this.setStatus("fail");
				return EMPTY;
			})).subscribe(({success}) => {
				this.setStatus(success ? "success" : "fail");
				if (success) {
					this.oldSsid = ssid;
					this.oldPassword = password;
				}
			});
		}
	}

	isDarkTheme() {
		return this.themeService.isDarkTheme();
	}

	setTheme(isDarkTheme: boolean) {
		this.themeService.setTheme(isDarkTheme);
	}

	private setStatus(status: "main" | "none" | "success" | "fail" = "main") {
		this.status = status;
		this.loading = false;
		if (status !== "main") {
			document.title = "WiFi Setup";
		}
	}
}
