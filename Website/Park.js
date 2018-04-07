var json = '{"result":true,"count":1}',
    obj = JSON.parse(json);

//alert(obj.count);

let url = 'https://example.com';

fetch(url)
.then(res => res.json())
.then((out) => {
  alert('Checkout this JSON! ', out);
})
.catch(err => { throw err });