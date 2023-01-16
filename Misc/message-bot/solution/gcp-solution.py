import gspread
from oauth2client.service_account import ServiceAccountCredentials


SCOPE = ['https://spreadsheets.google.com/feeds',
            'https://www.googleapis.com/auth/drive']
CREDS = ServiceAccountCredentials.from_json_keyfile_name('hidden-key.json', SCOPE)

# Show all files the service account has access to
def show_all_files():
    client = gspread.authorize(CREDS)
    files = client.list_spreadsheet_files()
    for file in files:
        print(file)


# Interesting spreadsheet is returned in the above function, so we can check it out
def read_cell():
    client = gspread.authorize(CREDS)
    sheet = client.open_by_key('1yB72pTgnWRlq-qe8pZRNOEr_0oA0onV7GVwY6JetwNk').sheet1
    cell = sheet.get('A1')  # Based on the public Display spreadsheet, the flag should be at A1
    print(cell)
