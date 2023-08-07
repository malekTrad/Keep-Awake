@app.route('/delete', method='POST')
def delete_product():
    product_id = request.forms.get('id')
    product_key = f'product:{product_id}'
       
    # Check if the product exists
    if r.exists(product_key):
        # Retrieve the product data
        product = r.hgetall(product_key)
        
        # Add a "deleted" field to the product data
        deletion_date = "2023-08-07"  # Use the actual deletion date
        product[b'deleted'] = deletion_date
        
        # Store the deleted product in a separate hash
        deleted_key = f'deleted_product:{product_id}'
        r.hmset(deleted_key, product)
        
        # Remove the product from the main products hash
        r.delete(product_key)
        
        return 'Product marked as deleted.'
    else:
        return 'Product not found.'

@app.route('/history')
def history():
    # Retrieve all product keys, including deleted ones
    all_keys = r.keys('*')

    # Retrieve product data for each key
    all_products = []
    for key in all_keys:
        if key.startswith('product:') or key.startswith('deleted_product:'):
            product = r.hgetall(key)
            all_products.append(product)

    return template('history', products=all_products)

