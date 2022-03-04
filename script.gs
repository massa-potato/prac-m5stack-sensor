function doPost(e){

  const date = new Date();

  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName('シート1');
  const params = JSON.parse(e.postData.getDataAsString());
  const [humidity, temperature, moisture] = [params.humidity, params.temperature, params.moisture];

  // データ追加
  sheet.insertRows(2, 1)
  sheet.getRange(2, 1, 1, 4).setValues([[date, humidity, temperature, moisture]]);

  const output = ContentService.createTextOutput();
  output.setMimeType(contentService.MimeType.JSON);
  output.setValue(JSON.stringify({ message: "success!" }));

  return output;
}
