import {Component} from "@angular/core";
import {MatInputModule} from "@angular/material/input";
import {MatButtonModule} from "@angular/material/button";
import {FormControl, FormGroup, ReactiveFormsModule} from "@angular/forms";
import {HttpClient} from "@angular/common/http";
import {MatProgressSpinnerModule} from "@angular/material/progress-spinner";
import {catchError, EMPTY} from "rxjs";

@Component({
	selector: "app-root",
	imports: [
		ReactiveFormsModule,
		MatInputModule,
		MatButtonModule,
		MatProgressSpinnerModule,
	],
	templateUrl: "./app.component.html",
	styleUrls: ["./app.component.css"],
})
export class AppComponent {
	protected status: "none" | "success" | "fail" = "none";
	protected loading = true;
	protected oldSsid = "";
	protected oldPassword = "";
	protected readonly formGroup;

	constructor(private readonly httpClient: HttpClient) {
		this.formGroup = new FormGroup({
			ssid: new FormControl(""),
			password: new FormControl(""),
		});
		this.httpClient.get<{ ssid: string, password: string }>("/api/wifi-read").pipe(catchError(() => {
			this.status = "fail";
			this.loading = false;
			return EMPTY;
		})).subscribe(({ssid, password}) => {
			this.formGroup.get("ssid")?.setValue(ssid);
			this.formGroup.get("password")?.setValue(password);
			this.loading = false;
		});
	}

	save() {
		const ssid = this.formGroup.getRawValue().ssid;
		const password = this.formGroup.getRawValue().password;

		if (ssid && password) {
			this.loading = true;
			this.httpClient.get<{ success: boolean }>(`/api/wifi-save?ssid=${(encodeURIComponent(ssid))}&password=${(encodeURIComponent(password))}`).pipe(catchError(() => {
				this.status = "fail";
				this.loading = false;
				return EMPTY;
			})).subscribe(({success}) => {
				this.status = success ? "success" : "fail";
				this.loading = false;
				if (success) {
					this.oldSsid = ssid;
					this.oldPassword = password;
				}
			});
		}
	}
}
