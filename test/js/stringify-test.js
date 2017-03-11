const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'stringify' });

test('stringify', function (t) {
  t.plan(2);
  t.type(bindings.stringify, 'function');
  t.equal(
    bindings.stringify({ "a": "JSON", "object": "value" }),
    JSON.stringify({ "a": "JSON", "object": "value" })
  );
});
