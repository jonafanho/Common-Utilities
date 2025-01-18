import {Injectable} from "@angular/core";

@Injectable({providedIn: "root"})
export class ThemeService {
	private darkTheme = false;

	constructor() {
		this.setTheme(ThemeService.getCookie("dark_theme") === "true");
	}

	public setTheme(isDarkTheme: boolean) {
		this.darkTheme = isDarkTheme;
		ThemeService.setCookie("dark_theme", isDarkTheme.toString());
		document.body.className = this.darkTheme ? "dark-theme" : "light-theme";
	}

	public isDarkTheme() {
		return this.darkTheme;
	}

	private static getCookie(name: string) {
		const splitCookies = document.cookie.split("; ").filter(cookie => cookie.startsWith(name + "="));
		if (splitCookies.length > 0 && splitCookies[0].includes("=")) {
			return decodeURIComponent(splitCookies[0].split("=")[1]);
		} else {
			return "";
		}
	}

	private static setCookie(name: string, value: string) {
		document.cookie = `${name}=${value}; expires=${new Date(2999, 11, 31).toUTCString()}; path=/`;
	}
}
