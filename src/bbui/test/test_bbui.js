function test_bbui() {
  module("BBUI");

  test("getCurrentTimeStr", function() {
    var now = getCurrentTimeStr();
    ok(now.match("\\d{4}/\\d{2}/\\d{2} \\d{2}:\\d{2}"),
       'getCurrentTimeStr');
  });

  test("zeroPadding", function() {
    for (var i = 0; i < 20; i++) {
      equals(zeroPadding(i).length, 2 , 'zeroPadding' + i)
    }
  });
}
